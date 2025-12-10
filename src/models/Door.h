#pragma once

#include <QString>
#include "../core/Types.h"

/**
 * @brief Door - Domain model for doors in the maze
 */
class Door {
public:
    Door() = default;
    Door(DoorType type, const QString& description = "")
        : m_type(type), m_description(description) {}

    // Getters
    DoorType getType() const { return m_type; }
    const QString& getDescription() const { return m_description; }
    bool isLocked() const { return m_type != DoorType::NORMAL; }

    // Setters
    void setType(DoorType type) { m_type = type; }
    void setDescription(const QString& desc) { m_description = desc; }

    // Convert to struct
    DoorData toDoorData() const {
        return {m_type, m_description};
    }

private:
    DoorType m_type = DoorType::NORMAL;
    QString m_description;
};
