#include "DatabaseManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QCoreApplication>
#include "../core/Constants.h"
#include <QDir>

DatabaseManager::DatabaseManager()
    : m_connection(std::make_unique<DatabaseConnection>())
{
}

DatabaseManager::~DatabaseManager() = default;

bool DatabaseManager::connect()
{
    if (!m_connection->connectSQLite("maze_game.db")) {
        m_lastError = m_connection->getLastError();
        qCritical() << "Failed to connect to SQLite:" << m_lastError;
        return false;
    }

    if (!isDatabaseInitialized()) {
        qDebug() << "Database not initialized. Loading from SQL file...";

        QString baseDir = QCoreApplication::applicationDirPath();
        QString sqlPath = baseDir + "/game_database.sql";

        if (!loadSqlFile("game_database.sql")) {
            m_lastError = "Failed to load database from SQL file";
            return false;
        }

        qDebug() << "Database initialized successfully!";
    } else {
        qDebug() << "Database already initialized";
    }

    return true;
}




bool DatabaseManager::isConnected() const
{
    return m_connection->isConnected();
}

bool DatabaseManager::isDatabaseInitialized()
{
    QSqlQuery query(m_connection->getDatabase());

    if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='locations'")) {
        qWarning() << "Failed to check database:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QSqlQuery countQuery(m_connection->getDatabase());
        if (countQuery.exec("SELECT COUNT(*) FROM locations") && countQuery.next()) {
            int count = countQuery.value(0).toInt();
            qDebug() << "Found" << count << "locations in database";
            return count > 0;
        }
    }

    qDebug() << "Database is not initialized";
    return false;
}

bool DatabaseManager::loadSqlFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open SQL file:" << filePath;
        qCritical() << "Current dir:" << QDir::currentPath();
        m_lastError = "Cannot open SQL file: " + filePath;
        return false;
    }

    QByteArray sqlData = file.readAll();
    file.close();

    QString sqlContent = QString::fromUtf8(sqlData);

    qDebug() << "SQL file loaded, size:" << sqlContent.length() << "bytes";

    // КРИТИЧНО: Принудительная замена AUTO_INCREMENT -> AUTOINCREMENT
    sqlContent.replace(QRegularExpression("AUTO_INCREMENT", QRegularExpression::CaseInsensitiveOption),
                      "AUTOINCREMENT");

    // Разбиваем на отдельные операторы вручную
    QStringList statements;
    QString currentStatement;
    bool inString = false;

    for (int i = 0; i < sqlContent.length(); i++) {
        QChar c = sqlContent[i];

        // Отслеживаем строки (чтобы не разбивать по ; внутри строк)
        if (c == '\'' && (i == 0 || sqlContent[i-1] != '\\')) {
            inString = !inString;
        }

        // Пропускаем комментарии --
        if (!inString && c == '-' && i + 1 < sqlContent.length() && sqlContent[i+1] == '-') {
            // Пропускаем до конца строки
            while (i < sqlContent.length() && sqlContent[i] != '\n') {
                i++;
            }
            continue;
        }

        currentStatement += c;

        // Разделяем по ; вне строк
        if (!inString && c == ';') {
            QString stmt = currentStatement.trimmed();
            if (!stmt.isEmpty() && !stmt.startsWith("--")) {
                statements.append(stmt);
            }
            currentStatement.clear();
        }
    }

    qDebug() << "Found" << statements.size() << "SQL statements";

    QSqlDatabase db = m_connection->getDatabase();

    if (!db.transaction()) {
        qWarning() << "Failed to start transaction:" << db.lastError().text();
    }

    QSqlQuery query(db);
    int successCount = 0;
    int failCount = 0;

    for (const QString &stmt : statements) {
        if (query.exec(stmt)) {
            successCount++;
            qDebug() << "✓ OK:" << stmt.left(60) + "...";
        } else {
            failCount++;
            qCritical() << "✗ FAIL:" << query.lastError().text();
            qCritical() << "Statement:" << stmt.left(150);
        }
    }

    if (!db.commit()) {
        qCritical() << "Failed to commit transaction:" << db.lastError().text();
        db.rollback();
        return false;
    }

    qDebug() << "SQL execution completed. Success:" << successCount << "Failed:" << failCount;

    return successCount > 0 && failCount == 0;
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

        QString rarityStr = query.value(4).toString();
        if (rarityStr == "LEGENDARY") {
            item.rarity = ItemRarity::LEGENDARY;
        } else if (rarityStr == "RARE") {
            item.rarity = ItemRarity::RARE;
        } else {
            item.rarity = ItemRarity::COMMON;
        }

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

    query.bindValue(":player", playerName);
    query.bindValue(":gold", goldBars);
    query.bindValue(":location", currentLocation);
    query.bindValue(":inventory", inventoryJson);
    query.bindValue(":logs", logsJson);

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
    query.bindValue(":player", playerName);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qCritical() << "Failed to load game state:" << m_lastError;
        return false;
    }

    if (query.next()) {
        qDebug() << "Game state loaded for player:" << playerName;
        qDebug() << "Gold bars:" << query.value(0).toInt();
        qDebug() << "Current location:" << query.value(1).toInt();
        return true;
    }

    qWarning() << "No saved game found for player:" << playerName;
    return false;
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}
