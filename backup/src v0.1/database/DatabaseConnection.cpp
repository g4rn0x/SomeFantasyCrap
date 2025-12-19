#include "DatabaseConnection.h"
#include <QtSql/QSqlError>
#include <QDebug>

int DatabaseConnection::s_connectionCounter = 0;

DatabaseConnection::DatabaseConnection()
{
    m_database = QSqlDatabase::addDatabase("QMYSQL", 
        QString("MazeGameConnection_%1").arg(++s_connectionCounter));
}

DatabaseConnection::~DatabaseConnection()
{
    disconnect();
}

bool DatabaseConnection::connect(const QString& host, const QString& dbName,
                                 const QString& user, const QString& password, int port)
{
    if (m_database.isOpen()) {
        qWarning() << "Database is already connected";
        return true;
    }

    m_database.setHostName(host);
    m_database.setDatabaseName(dbName);
    m_database.setUserName(user);
    m_database.setPassword(password);
    m_database.setPort(port);

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qCritical() << "Database connection failed:" << m_lastError;
        return false;
    }

    qDebug() << "Database connected successfully";
    return true;
}

bool DatabaseConnection::isConnected() const
{
    return m_database.isOpen();
}

void DatabaseConnection::disconnect()
{
    if (m_database.isOpen()) {
        m_database.close();
        qDebug() << "Database disconnected";
    }
}

QSqlDatabase& DatabaseConnection::getDatabase()
{
    return m_database;
}

const QSqlDatabase& DatabaseConnection::getDatabase() const
{
    return m_database;
}

QString DatabaseConnection::getLastError() const
{
    return m_lastError;
}
