#pragma once

#include <QString>
#include <QVector>
#include <memory>

enum class DoorType {
    NORMAL,
    SILVER,
    GOLD
};
enum class ItemType {
    SILVER_KEY,
    GOLD_KEY
};
enum class ItemRarity {
    COMMON,
    RARE,
    LEGENDARY
};
struct LocationData {
    int id;
    QString name;
    QString theme;
    QString description;
};
struct ItemData {
    int id;
    ItemType type;
    QString name;
    QString description;
    ItemRarity rarity;
};
struct RiddleData {
    int id;
    QString question;
    QString answer;
    int difficulty;
};
struct NoteData {
    int id;
    QString content;
    int locationId;
};
struct DoorData {
    DoorType type;
    QString description;
};
inline QString doorTypeToString(DoorType type) {
    switch (type) {
        case DoorType::NORMAL: return "Обычная";
        case DoorType::SILVER: return "Серебряная";
        case DoorType::GOLD: return "Золотая";
    }
    return "";
}
inline QString itemTypeToString(ItemType type) {
    switch (type) {
        case ItemType::SILVER_KEY: return "Серебряный ключ";
        case ItemType::GOLD_KEY: return "Золотой ключ";
    }
    return "";
}
