#pragma once

#include <QString>
#include "../core/Types.h"

/**
 * @brief Item - Domain model for inventory items
 */
class Item {
public:
    Item() = default;
    Item(int id, ItemType type, const QString& name, const QString& description, ItemRarity rarity)
        : m_id(id), m_type(type), m_name(name), m_description(description), m_rarity(rarity) {}

    // Getters
    int getId() const { return m_id; }
    ItemType getType() const { return m_type; }
    const QString& getName() const { return m_name; }
    const QString& getDescription() const { return m_description; }
    ItemRarity getRarity() const { return m_rarity; }

    // Setters
    void setName(const QString& name) { m_name = name; }
    void setDescription(const QString& desc) { m_description = desc; }
    void setRarity(ItemRarity rarity) { m_rarity = rarity; }

    // Convert to struct
    ItemData toItemData() const {
        return {m_id, m_type, m_name, m_description, m_rarity};
    }

private:
    int m_id = -1;
    ItemType m_type = ItemType::SILVER_KEY;
    QString m_name;
    QString m_description;
    ItemRarity m_rarity = ItemRarity::COMMON;
};
