#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>
#include <QSqlDatabase>

class DatabaseConnection {
public:
    DatabaseConnection();
    ~DatabaseConnection();

    bool connectSQLite(const QString &databasePath);
    bool connect(const QString &host, const QString &database,
                const QString &user, const QString &password, int port = 3306);

    bool isConnected() const;
    QSqlDatabase getDatabase();
    QString getLastError() const;
    void disconnect();

private:
    QSqlDatabase m_database;
    bool m_connected;
    QString m_lastError;
};

#endif // DATABASECONNECTION_H

