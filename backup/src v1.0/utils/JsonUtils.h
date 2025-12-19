#pragma once

#include <QString>
#include <QVector>

/**
 * @brief JsonUtils - Utility functions for JSON serialization
 */
class JsonUtils {
public:
    JsonUtils() = delete;

    // Serialize game data to JSON
    static QString inventoryToJson(const QVector<int>& inventory);
    static QString logsToJson(const QVector<QString>& logs);

    // Deserialize game data from JSON
    static QVector<int> inventoryFromJson(const QString& json);
    static QVector<QString> logsFromJson(const QString& json);
};
