#include "JsonUtils.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

QString JsonUtils::inventoryToJson(const QVector<int>& inventory)
{
    QJsonArray array;
    for (int item : inventory) {
        array.append(item);
    }
    QJsonDocument doc(array);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

QString JsonUtils::logsToJson(const QVector<QString>& logs)
{
    QJsonArray array;
    for (const QString& log : logs) {
        array.append(log);
    }
    QJsonDocument doc(array);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

QVector<int> JsonUtils::inventoryFromJson(const QString& json)
{
    QVector<int> inventory;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    
    if (!doc.isArray()) {
        qWarning() << "Invalid inventory JSON format";
        return inventory;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue& value : array) {
        if (value.isDouble()) {
            inventory.append(static_cast<int>(value.toDouble()));
        }
    }
    return inventory;
}

QVector<QString> JsonUtils::logsFromJson(const QString& json)
{
    QVector<QString> logs;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    
    if (!doc.isArray()) {
        qWarning() << "Invalid logs JSON format";
        return logs;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue& value : array) {
        if (value.isString()) {
            logs.append(value.toString());
        }
    }
    return logs;
}
