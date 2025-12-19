#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QVector>
#include <memory>
#include "DatabaseConnection.h"
#include "../core/Types.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect();
    bool isConnected() const;

    QVector<LocationData> loadLocations();
    QVector<RiddleData> loadRiddles();
    QVector<NoteData> loadNotes();
    QVector<ItemData> loadItems();

    bool saveGameState(const QString& playerName, int goldBars, int currentLocation,
                      const QString& inventoryJson, const QString& logsJson);
    bool loadGameState(const QString& playerName);

    QString getLastError() const;

private:
    std::unique_ptr<DatabaseConnection> m_connection;
    QString m_lastError;

    bool isDatabaseInitialized();
    bool loadSqlFile(const QString &filePath);
};

#endif // DATABASEMANAGER_H
