#include "GameEngine.h"
#include "../database/DatabaseManager.h"
#include "../utils/RandomGenerator.h"
#include "../utils/TextGenerator.h"
#include "Constants.h"
#include <QDebug>

GameEngine::GameEngine(QObject* parent)
    : QObject(parent)
    , m_database(std::make_unique<DatabaseManager>())
    , m_movesRemaining(MOVES_PER_LOCATION)
{
    RandomGenerator::initializeSeed();
}

GameEngine::~GameEngine() = default;

void GameEngine::initializeGame()
{
    if (!m_database->connect()) {
        emit errorOccurred("Не удалось подключиться к базе данных");
        return;
    }

    m_locations = m_database->loadLocations();
    m_riddles = m_database->loadRiddles();
    m_notes = m_database->loadNotes();

    if (m_locations.size() > 1) {
        for (int i = m_locations.size() - 1; i > 0; --i) {
            int j = RandomGenerator::random(0, i - 1);
            std::swap(m_locations[i], m_locations[j]);
        }
    }

    if (m_locations.isEmpty()) {
        emit errorOccurred("Локации не загружены из БД");
        return;
    }

    m_currentState.setCurrentLocationIndex(0)
                   .setCurrentRoomIndex(0)
                   .setGoldBars(0)
                   .setGameOver(false)
                   .setGameWon(false);

    m_currentState.addLog("Добро пожаловать в Лабиринт!");
    m_currentState.addLog("Выберите дверь, чтобы начать приключение.");

    m_currentState.setCurrentDoors(generateDoors());
    generateRoomDescription(m_currentState);

    emit gameInitialized(m_currentState);
}

void GameEngine::onDoorSelected(int doorIndex)
{

    GameState newState = processMove(m_currentState, doorIndex);
    m_currentState = newState;
    emit gameStateChanged(m_currentState);
}

void GameEngine::addFoundNote(const NoteData& note)
{
    m_totalNotesFound++;
    m_currentState.addLog(QString("[*] Записок найдено: %1").arg(m_totalNotesFound));
}

void GameEngine::startRoomDescriptionTypeWriter(const QString& text)
{
    if (!m_typeWriter) {
        m_typeWriter = std::make_unique<TypeWriter>();
        connect(m_typeWriter.get(), &TypeWriter::characterAdded,
                this, &GameEngine::onTypeWriterCharacterAdded);
        connect(m_typeWriter.get(), &TypeWriter::typingFinished,
                this, &GameEngine::onTypeWriterFinished);
    }

    m_typeWriter->startTyping(text, 50);
    emit typeWriterStarted(text);
}

void GameEngine::skipCurrentTypeWriter()
{
    if (m_typeWriter && m_typeWriter->isTyping()) {
        m_typeWriter->skipToEnd();
    }
}

void GameEngine::onTypeWriterCharacterAdded(const QString& text)
{
    emit roomDescriptionUpdated(text);
}

void GameEngine::onTypeWriterFinished()
{
    emit typeWriterFinished();
}

GameState GameEngine::processMove(const GameState& currentState, int doorIndex)
{
    if (currentState.isGameOver()) {
        return currentState;
    }
    GameState newState = currentState;
    newState.setLoading(true);

    if (doorIndex < 0 || doorIndex >= newState.getCurrentDoors().size()) {
        newState.addLog("ОШИБКА: Неверный выбор двери");
        newState.setLoading(false);
        return newState;
    }

    const DoorData& door = newState.getCurrentDoors()[doorIndex];

    if (!processKeyRequirement(newState, door)) {
        newState.setLoading(false);
        return newState;
    }

    int newRoomIndex = newState.getCurrentRoomIndex() + 1;
    newState.setCurrentRoomIndex(newRoomIndex);
    newState.addLog(QString("Вы вошли в комнату %1/10").arg(newRoomIndex));

    if (newRoomIndex >= MOVES_PER_LOCATION) {
        handleLocationTransition(newState);
        newState.setCurrentDoors(generateDoors());
        generateRoomDescription(newState);
        handleEventGeneration(newState, newState.getCurrentDoors().first());

        newState.setLoading(false);
        return newState;
    }

    if (checkWinCondition(newState)) {
        newState.setGameWon(true);
        newState.setGameOver(true);
        emit gameWon(m_totalNotesFound, newState.getGoldBars());
        return newState;
    }

    if (newRoomIndex < MOVES_PER_LOCATION) {
        auto doors = generateDoors();
        newState.setCurrentDoors(doors);

        if (!doors.isEmpty()) {
            handleEventGeneration(newState, doors.first());

            if (newState.getActiveRiddle()) {
                newState.setLoading(false);
                return newState;
            }
        }

        generateRoomDescription(newState);
    }

    newState.setLoading(false);
    return newState;
}

void GameEngine::handleRiddleAnswer(const QString& answer)
{
    qDebug() << "handleRiddleAnswer called";

    if (!m_currentRiddle) {
        emit errorOccurred("Нет активной загадки");
        return;
    }

    GameState newState = m_currentState;

    QString normalizedAnswer = answer.toLower().trimmed();
    QString correctAnswer = m_currentRiddle->answer.toLower().trimmed();

    qDebug() << "Riddle answer given:" << normalizedAnswer
             << "correct:" << correctAnswer;

    if (normalizedAnswer == correctAnswer) {
        newState.addLog("╔════════════════════════════════════════╗");
        newState.addLog("║  ✓ ПРАВИЛЬНО! Загадка разгадана!      ║");
        newState.addLog("╚════════════════════════════════════════╝");

        if (newState.hasInventorySpace()) {
            newState.addItem(ItemType::GOLD_KEY);
            newState.addLog("🗝️  Вы получили: Золотой ключ");
        } else {
            newState.addLog("⚠️  Инвентарь полон! Золотой ключ утерян.");
        }
    } else {
        newState.addLog("╔════════════════════════════════════════╗");
        newState.addLog("║  ✗ НЕПРАВИЛЬНО!                       ║");
        newState.addLog("╚════════════════════════════════════════╝");
        newState.addLog(QString("Правильный ответ: %1").arg(m_currentRiddle->answer));
    }

    newState.setActiveRiddle(nullptr);
    m_currentRiddle = nullptr;

    generateRoomDescription(newState);

    m_currentState = newState;

    qDebug() << "Emitting gameStateChanged with logs count:" << newState.getLogs().size();
    emit gameStateChanged(newState);
}

bool GameEngine::checkWinCondition(const GameState& state) const
{

    return state.getCurrentLocationIndex() >= m_locations.size();
}

bool GameEngine::hasGameEnded(const GameState& state) const
{
    return state.isGameOver();
}

QVector<DoorData> GameEngine::generateDoors() const
{
    QVector<DoorData> doors;
    int doorCount = RandomGenerator::random(2, 4);
    doors.append({DoorType::NORMAL, "Обычная деревянная дверь"});

    for (int i = 1; i < doorCount; ++i) {
        double rand = RandomGenerator::randomDouble();
        DoorType type;

        if (rand < 0.4) {
            type = DoorType::SILVER;
        } else if (rand < 0.7) {
            type = DoorType::GOLD;
        } else {
            type = DoorType::NORMAL;
        }

        doors.append({type, doorTypeToString(type) + " дверь"});
    }

    if (doors.size() > 1) {
        for (int i = doors.size() - 1; i > 0; --i) {
            int j = RandomGenerator::random(0, i - 1);
            std::swap(doors[i], doors[j]);
        }
    }
    return doors;
}

bool GameEngine::processKeyRequirement(GameState& state, const DoorData& door)
{
    if (door.type == DoorType::SILVER) {
        if (!state.hasItem(ItemType::SILVER_KEY)) {
            state.addLog("⚠️ Дверь заперта! Нужен серебряный ключ.");
            return false;
        }
        state.removeItem(ItemType::SILVER_KEY);
        state.addLog("🔑 Вы открыли серебряную дверь!");
        return true;
    }

    if (door.type == DoorType::GOLD) {
        if (!state.hasItem(ItemType::GOLD_KEY)) {
            state.addLog("⚠️ Дверь заперта! Нужен золотой ключ.");
            return false;
        }
        state.removeItem(ItemType::GOLD_KEY);
        state.addLog("✨ Вы открыли золотую дверь!");
        state.setGoldBars(state.getGoldBars() + 1);
        state.addLog(QString("💰 Золотых слитков: %1").arg(state.getGoldBars()));
        return true;
    }

    state.addLog("Вы прошли через обычную дверь.");
    return true;
}

void GameEngine::handleLocationTransition(GameState& state)
{
    int nextLocation = state.getCurrentLocationIndex() + 1;
    state.setCurrentLocationIndex(nextLocation);
    state.setCurrentRoomIndex(0);
    state.addLog("");
    state.addLog("╔════════════════════════════════════════╗");
    state.addLog(QString("║  ЛОКАЦИЯ ПРОЙДЕНА! Уровень %1 завершён  ║").arg(nextLocation));
    state.addLog("╚════════════════════════════════════════╝");
    state.addLog("");
}

void GameEngine::handleEventGeneration(GameState& state, const DoorData& door)
{
    double eventRoll = RandomGenerator::randomDouble();

    double noteChance = 0.4;
    double itemChance = 0.25;
    double riddleChance = 0.1;

    if (door.type == DoorType::SILVER) {
        itemChance += 0.15;
        riddleChance += 0.05;
    }

    if (eventRoll < noteChance && !m_notes.isEmpty()) {
        NoteData note = m_notes.takeFirst();
        state.addNote(note);
        addFoundNote(note);
        state.addLog(QString("На полу найдена записка: \"%1\"").arg(note.content.left(30)));
        emit noteFound(note);
        return;
    }

    if (eventRoll < noteChance + itemChance) {
        if (state.hasInventorySpace()) {
            bool isSilverDoor = (door.type == DoorType::SILVER);
            ItemType item = randomItem(isSilverDoor);  // Генерация случайного предмета
            state.addItem(item);
            state.addLog(QString(" Вы нашли: %1").arg(itemTypeToString(item)));
        } else {
            state.addLog(" Вы нашли ключ, но инвентарь полон!");
        }
        return;
    }

    if (eventRoll < noteChance + itemChance + riddleChance && !m_riddles.isEmpty()) {
        RiddleData riddle = m_riddles.takeFirst();
        m_currentRiddle = std::make_shared<RiddleData>(riddle);
        state.setActiveRiddle(m_currentRiddle);

        state.addLog("");
        state.addLog("⚡ ПУТЬ ПРЕГРАЖДАЕТ ЗАГАДОЧНИК!");
        state.addLog(QString("Загадка: %1").arg(m_currentRiddle->question));
        state.addLog("");

        emit riddleEncountered(riddle);
        return;
    }
}


QString GameEngine::getGeneratedRoomDescription(int locationId, int roomNumber)
{
    if (locationId < m_locations.size() && locationId >= 0) {
        const LocationData& loc = m_locations[locationId];
        return TextGenerator::generateRoomDescription(
            locationId + 1,
            roomNumber,
            loc.name,
            loc.theme
        );
    }
    return "Вы входите в комнату...";
}

void GameEngine::generateRoomDescription(GameState& state)
{
    if (state.getCurrentLocationIndex() < m_locations.size()) {
        const LocationData& loc = m_locations[state.getCurrentLocationIndex()];

        QString description = TextGenerator::generateRoomDescription(
            state.getCurrentLocationIndex() + 1,
            state.getCurrentRoomIndex() + 1,
            loc.name,
            loc.theme
        );

        state.setRoomDescription(description);

        QString imagePath = QString(":/assets/locations/location_%1.png").arg(state.getCurrentLocationIndex() + 1);
        state.setLocationImagePath(imagePath);

        startRoomDescriptionTypeWriter(description);
    }
}

ItemType GameEngine::randomItem(bool isSilverDoor) const
{
    double goldKeyChance = isSilverDoor ? 0.4 : 0.2;
    return RandomGenerator::randomDouble() < goldKeyChance
        ? ItemType::GOLD_KEY
        : ItemType::SILVER_KEY;
}
