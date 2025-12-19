#include "DatabaseConnection.h"
#include <QSqlError>
#include <QDebug>

DatabaseConnection::DatabaseConnection()
    : m_connected(false)
{
}

DatabaseConnection::~DatabaseConnection()
{
    if (m_connected) {
        m_database.close();
    }
}

bool DatabaseConnection::connectSQLite(const QString &databasePath)
{
    // Если уже подключены, закрываем старое соединение
    if (m_connected && m_database.isOpen()) {
        m_database.close();
        m_connected = false;
    }

    // Удаляем старое соединение если оно существует
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(databasePath);

    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qCritical() << "SQLite connection failed:" << m_lastError;
        return false;
    }

    m_connected = true;
    qDebug() << "Connected to SQLite database:" << databasePath;
    return true;
}


bool DatabaseConnection::connect(const QString &host, const QString &database,
                                const QString &user, const QString &password, int port)
{
    if (m_connected) {
        return true;
    }

    m_database = QSqlDatabase::addDatabase("QMYSQL");
    m_database.setHostName(host);
    m_database.setDatabaseName(database);
    m_database.setUserName(user);
    m_database.setPassword(password);
    m_database.setPort(port);

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qCritical() << "MySQL connection failed:" << m_lastError;
        return false;
    }

    m_connected = true;
    qDebug() << "Connected to MySQL database:" << database;
    return true;
}

bool DatabaseConnection::isConnected() const
{
    return m_connected && m_database.isOpen();
}

QSqlDatabase DatabaseConnection::getDatabase()
{
    return m_database;
}

QString DatabaseConnection::getLastError() const
{
    return m_lastError;
}

void DatabaseConnection::disconnect()
{
    if (m_connected) {
        QString connectionName = m_database.connectionName();
        m_database.close();
        m_connected = false;

        // Удаляем соединение из пула Qt
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase::removeDatabase(connectionName);
        }

        qDebug() << "Database disconnected";
    }
}

