#pragma once

#include <QString>
#include <QVector>
#include <memory>
#include "../core/Types.h"
#include "DatabaseConnection.h"

/**
 * @brief DatabaseManager - High-level database operations
 * Responsible for: game data loading/saving, queries execution
 * Uses DatabaseConnection for low-level operations
 */
class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    // Connection
    bool connect();
    bool isConnected() const;

    // Data loading (read operations)
    QVector<LocationData> loadLocations();
    QVector<RiddleData> loadRiddles();
    QVector<NoteData> loadNotes();
    QVector<ItemData> loadItems();

    // Game state operations
    bool saveGameState(const QString& playerName, int goldBars, int currentLocation,
                       const QString& inventoryJson, const QString& logsJson);
    bool loadGameState(const QString& playerName);

    // Error handling
    QString getLastError() const { return m_lastError; }

private:
    // Helper methods
    bool createTablesIfNotExist();
    bool initializeDefaultData();
    
    std::unique_ptr<DatabaseConnection> m_connection;
    QString m_lastError;
};
