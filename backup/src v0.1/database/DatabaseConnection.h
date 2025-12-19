#pragma once

#include <QString>
#include <QSqlDatabase>

/**
 * @brief DatabaseConnection - Handles low-level database connection
 * Single Responsibility: Manage database connection lifecycle
 */
class DatabaseConnection {
public:
    DatabaseConnection();
    ~DatabaseConnection();

    // Connection management
    bool connect(const QString& host, const QString& dbName, 
                 const QString& user, const QString& password, int port = 3306);
    bool isConnected() const;
    void disconnect();
    
    // Connection access
    QSqlDatabase& getDatabase();
    const QSqlDatabase& getDatabase() const;
    
    // Error handling
    QString getLastError() const;

private:
    QSqlDatabase m_database;
    QString m_lastError;
    static int s_connectionCounter;
};
