#include "DatabaseManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include "../core/Constants.h"

DatabaseManager::DatabaseManager()
    : m_connection(std::make_unique<DatabaseConnection>())
{
}

DatabaseManager::~DatabaseManager() = default;

bool DatabaseManager::connect()
{
    if (!m_connection->connect(DB_HOST, DB_NAME, DB_USER, DB_PASSWORD, DB_PORT)) {
        m_lastError = m_connection->getLastError();
        return false;
    }

    if (!createTablesIfNotExist()) {
        m_lastError = "Failed to create database tables";
        return false;
    }

    if (!initializeDefaultData()) {
        qWarning() << "Warning: Could not initialize default data";
    }

    return true;
}

bool DatabaseManager::isConnected() const
{
    return m_connection->isConnected();
}

QVector<LocationData> DatabaseManager::loadLocations()
{
    QVector<LocationData> locations;

    QSqlQuery query(m_connection->getDatabase());
    query.prepare("SELECT id, name, theme, description FROM locations ORDER BY id");

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qCritical() << "Failed to load locations:" << m_lastError;
        return locations;
    }

    while (query.next()) {
        LocationData loc;
        loc.id = query.value(0).toInt();
        loc.name = query.value(1).toString();
        loc.theme = query.value(2).toString();
        loc.description = query.value(3).toString();
        locations.append(loc);
    }

    qDebug() << "Loaded" << locations.size() << "locations from database";
    return locations;
}

QVector<RiddleData> DatabaseManager::loadRiddles()
{
    QVector<RiddleData> riddles;

    QSqlQuery query(m_connection->getDatabase());
    query.prepare("SELECT id, question, answer, difficulty FROM riddles ORDER BY difficulty");

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qCritical() << "Failed to load riddles:" << m_lastError;
        return riddles;
    }

    while (query.next()) {
        RiddleData riddle;
        riddle.id = query.value(0).toInt();
        riddle.question = query.value(1).toString();
        riddle.answer = query.value(2).toString().toLower();
        riddle.difficulty = query.value(3).toInt();
        riddles.append(riddle);
    }

    qDebug() << "Loaded" << riddles.size() << "riddles from database";
    return riddles;
}

QVector<NoteData> DatabaseManager::loadNotes()
{
    QVector<NoteData> notes;

    QSqlQuery query(m_connection->getDatabase());
    query.prepare("SELECT id, content, location_id FROM notes ORDER BY id");

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qCritical() << "Failed to load notes:" << m_lastError;
        return notes;
    }

    while (query.next()) {
        NoteData note;
        note.id = query.value(0).toInt();
        note.content = query.value(1).toString();
        note.locationId = query.value(2).toInt();
        notes.append(note);
    }

    qDebug() << "Loaded" << notes.size() << "notes from database";
    return notes;
}

QVector<ItemData> DatabaseManager::loadItems()
{
    QVector<ItemData> items;

    QSqlQuery query(m_connection->getDatabase());
    query.prepare("SELECT id, type, name, description, rarity FROM items");

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qCritical() << "Failed to load items:" << m_lastError;
        return items;
    }

    while (query.next()) {
        ItemData item;
        item.id = query.value(0).toInt();
        QString typeStr = query.value(1).toString();
        item.type = (typeStr == "GOLD_KEY") ? ItemType::GOLD_KEY : ItemType::SILVER_KEY;
        item.name = query.value(2).toString();
        item.description = query.value(3).toString();
        item.rarity = ItemRarity::COMMON;
        items.append(item);
    }

    qDebug() << "Loaded" << items.size() << "items from database";
    return items;
}

bool DatabaseManager::saveGameState(const QString& playerName, int goldBars, int currentLocation,
                                    const QString& inventoryJson, const QString& logsJson)
{
    QSqlQuery query(m_connection->getDatabase());
    query.prepare("INSERT INTO game_saves (player_name, gold_bars, current_location, inventory, logs) "
                  "VALUES (:player, :gold, :location, :inventory, :logs)");

    query.addBindValue(playerName);
    query.addBindValue(goldBars);
    query.addBindValue(currentLocation);
    query.addBindValue(inventoryJson);
    query.addBindValue(logsJson);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qCritical() << "Failed to save game state:" << m_lastError;
        return false;
    }

    qDebug() << "Game state saved for player:" << playerName;
    return true;
}

bool DatabaseManager::loadGameState(const QString& playerName)
{
    QSqlQuery query(m_connection->getDatabase());
    query.prepare("SELECT gold_bars, current_location, inventory, logs FROM game_saves "
                  "WHERE player_name = :player ORDER BY saved_at DESC LIMIT 1");
    query.addBindValue(playerName);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qCritical() << "Failed to load game state:" << m_lastError;
        return false;
    }

    if (query.next()) {
        qDebug() << "Game state loaded for player:" << playerName;
        return true;
    }

    return false;
}

bool DatabaseManager::createTablesIfNotExist()
{
    QSqlQuery query(m_connection->getDatabase());

    // Locations table
    if (!query.exec("CREATE TABLE IF NOT EXISTS locations ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "name VARCHAR(255) NOT NULL,"
                    "theme VARCHAR(255),"
                    "description TEXT,"
                    "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Riddles table
    if (!query.exec("CREATE TABLE IF NOT EXISTS riddles ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "question TEXT NOT NULL,"
                    "answer VARCHAR(255) NOT NULL,"
                    "difficulty INT DEFAULT 1,"
                    "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Notes table
    if (!query.exec("CREATE TABLE IF NOT EXISTS notes ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "content TEXT NOT NULL,"
                    "location_id INT,"
                    "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                    "FOREIGN KEY (location_id) REFERENCES locations(id))")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Items table
    if (!query.exec("CREATE TABLE IF NOT EXISTS items ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "type VARCHAR(50) NOT NULL,"
                    "name VARCHAR(255),"
                    "description TEXT,"
                    "rarity VARCHAR(50) DEFAULT 'COMMON')")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Game saves table
    if (!query.exec("CREATE TABLE IF NOT EXISTS game_saves ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "player_name VARCHAR(255),"
                    "gold_bars INT DEFAULT 0,"
                    "current_location INT DEFAULT 0,"
                    "inventory JSON,"
                    "logs JSON,"
                    "saved_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)")) {
        m_lastError = query.lastError().text();
        return false;
    }

    qDebug() << "Database tables created/verified successfully";
    return true;
}

bool DatabaseManager::initializeDefaultData()
{
    QSqlQuery query(m_connection->getDatabase());

    // Check if data already exists
    query.prepare("SELECT COUNT(*) FROM locations");
    if (!query.exec()) {
        return false;
    }
    query.next();
    if (query.value(0).toInt() > 0) {
        return true; // Data already exists
    }

    // Insert default locations
    query.prepare("INSERT INTO locations (name, theme, description) VALUES (:name, :theme, :desc)");

    QVector<std::tuple<QString, QString, QString>> locations = {
        {"Древний Замок", "Готика", "Массивный замок с темными коридорами"},
        {"Подземелье Ордена", "Мистика", "Священное подземелье с загадочными символами"},
        {"Забытый Город", "Упадок", "Развалины некогда величественного города"},
        {"Лес Теней", "Природа", "Древний лес со светящимися деревьями"},
        {"Кристальный Дворец", "Фантазия", "Сверкающий дворец из драгоценных камней"}
    };

    for (const auto& [name, theme, desc] : locations) {
        query.addBindValue(name);
        query.addBindValue(theme);
        query.addBindValue(desc);
        if (!query.exec()) {
            return false;
        }
    }

    // Insert default riddles
    query.prepare("INSERT INTO riddles (question, answer, difficulty) VALUES (:q, :a, :d)");

    QVector<std::tuple<QString, QString, int>> riddles = {
        {"Что имеет города, но нет домов?", "карта", 1},
        {"Я невидимка, но все ощущают мой присутствие. Кто я?", "ветер", 1},
        {"Чем больше ты берешь, тем больше становится. Что это?", "яма", 2},
        {"У какого животного ноги растут на спине?", "осёл", 2},
        {"Я говорю без уст и слышу без ушей. Что я?", "эхо", 2}
    };

    for (const auto& [question, answer, difficulty] : riddles) {
        query.addBindValue(question);
        query.addBindValue(answer);
        query.addBindValue(difficulty);
        if (!query.exec()) {
            return false;
        }
    }

    qDebug() << "Default database data initialized";
    return true;
}
