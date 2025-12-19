#include "GameEngine.h"
#include "../database/DatabaseManager.h"
#include "../utils/RandomGenerator.h"
#include "Constants.h"
#include <QDebug>

GameEngine::GameEngine(QObject* parent)
    : QObject(parent)  // Initialize QObject first
    , m_database(std::make_unique<DatabaseManager>())
    , m_movesRemaining(MOVES_PER_LOCATION)
{
    RandomGenerator::initializeSeed();
}

GameEngine::~GameEngine() = default;

void GameEngine::initializeGame()
{
    // Initialize database
    if (!m_database->connect()) {
        emit errorOccurred("Не удалось подключиться к базе данных");
        return;
    }

    // Load game data from database
    m_locations = m_database->loadLocations();
    m_riddles = m_database->loadRiddles();
    m_notes = m_database->loadNotes();

    if (m_locations.isEmpty()) {
        emit errorOccurred("Локации не загружены из БД");
        return;
    }

    // Initialize game state
    m_currentState.setCurrentLocationIndex(0)
                   .setCurrentRoomIndex(0)
                   .setGoldBars(0)
                   .setGameOver(false)
                   .setGameWon(false);

    m_currentState.addLog("Добро пожаловать в Лабиринт!");
    m_currentState.addLog("Выберите дверь, чтобы начать приключение.");

    // Generate first room
    m_currentState.setCurrentDoors(generateDoors());
    generateRoomDescription(m_currentState);

    emit gameInitialized(m_currentState);
}

void GameEngine::onDoorSelected(int doorIndex)
{
    // Используем текущий state
    GameState newState = processMove(m_currentState, doorIndex);
    m_currentState = newState;          // обновляем внутреннее состояние
    emit gameStateChanged(m_currentState);
}


GameState GameEngine::processMove(const GameState& currentState, int doorIndex)
{
    GameState newState = currentState;
    newState.setLoading(true);

    if (doorIndex < 0 || doorIndex >= newState.getCurrentDoors().size()) {
        newState.addLog("ОШИБКА: Неверный выбор двери");
        newState.setLoading(false);
        return newState;
    }

    const DoorData& door = newState.getCurrentDoors()[doorIndex];

    // Process key requirements
    if (!processKeyRequirement(newState, door)) {
        newState.setLoading(false);
        return newState;
    }

    // Advance room
    int newRoomIndex = newState.getCurrentRoomIndex() + 1;
    newState.setCurrentRoomIndex(newRoomIndex);
    newState.addLog(QString("Вы вошли в комнату %1/10").arg(newRoomIndex));

    // Check location transition
    if (newRoomIndex >= MOVES_PER_LOCATION) {
        handleLocationTransition(newState);
    }

    // Check win condition
    if (checkWinCondition(newState)) {
        newState.setGameWon(true);
        newState.setGameOver(true);
        newState.addLog("╔════════════════════════════════════════╗");
        newState.addLog("║       ПОБЕДА! Вы прошли все уровни!   ║");
        newState.addLog("╚════════════════════════════════════════╝");
        newState.setLoading(false);
        return newState;
    }

    // Generate new room if still in location
    if (newRoomIndex < MOVES_PER_LOCATION) {
        newState.setCurrentDoors(generateDoors());
        handleEventGeneration(newState);
        generateRoomDescription(newState);
    }

    newState.setLoading(false);
    return newState;
}

void GameEngine::handleRiddleAnswer(const GameState& currentState, const QString& answer)
{
    GameState newState = currentState;
    
    const RiddleData* riddle = newState.getActiveRiddle();
    if (!riddle) {
        emit errorOccurred("Нет активной загадки");
        return;
    }

    if (answer.toLower().trimmed() == riddle->answer.toLower().trimmed()) {
        newState.addLog("✓ Правильно! Вы прошли мимо стража.");
        newState.setActiveRiddle(nullptr);
    } else {
        newState.addLog(QString("✗ Неправильно. Ответ: %1").arg(riddle->answer));
        newState.setActiveRiddle(nullptr);
    }

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

    // Always at least one normal door
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

    // Shuffle doors
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

void GameEngine::handleEventGeneration(GameState& state)
{
    double eventRoll = RandomGenerator::randomDouble();

    // Event: Note (10% chance)
    if (eventRoll < EVENT_NOTE_CHANCE && !m_notes.isEmpty()) {
        NoteData note = m_notes.takeFirst();
        state.addNote(note);
        state.addLog(QString("На полу найдена записка: \"%1\"").arg(note.content));
        return;
    }

    // Event: Item (20% chance)
    if (eventRoll < EVENT_ITEM_CHANCE) {
        if (state.hasInventorySpace()) {
            ItemType item = randomItem();
            state.addItem(item);
            state.addLog(QString("🎁 Вы нашли: %1").arg(itemTypeToString(item)));
        } else {
            state.addLog("🎁 Вы нашли ключ, но инвентарь полон!");
        }
        return;
    }

    // Event: Riddle (15% chance)
    if (eventRoll > 0.85 && !m_riddles.isEmpty()) {
        RiddleData riddle = m_riddles.takeFirst();
        state.setActiveRiddle(std::make_shared<RiddleData>(riddle));
        state.addLog("");
        state.addLog("🧩 ПУТЬ ПРЕГРАЖДАЕТ ЗАГАДОЧНИК!");
        state.addLog(QString("Загадка: %1").arg(riddle.question));
    }
}

void GameEngine::generateRoomDescription(GameState& state)
{
    if (state.getCurrentLocationIndex() < m_locations.size()) {
        const LocationData& loc = m_locations[state.getCurrentLocationIndex()];
        QString description = QString(
            "Вы входите в локацию \"%1\" (Тема: %2).\n"
            "Это комната номер %3/10 на этаже. "
            "Атмосфера мрачная и таинственная. "
            "Вокруг тишина, только эхо ваших шагов."
        ).arg(loc.name, loc.theme, QString::number(state.getCurrentRoomIndex() + 1));
        
        state.setRoomDescription(description);
    }
}

ItemType GameEngine::randomItem() const
{
    return RandomGenerator::randomDouble() > 0.5
        ? ItemType::SILVER_KEY
        : ItemType::GOLD_KEY;
}
