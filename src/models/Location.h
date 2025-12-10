#pragma once

#include <QString>
#include "../core/Types.h"

/**
 * @brief Location - Domain model for game location
 */
class Location {
public:
    Location() = default;
    Location(int id, const QString& name, const QString& theme, const QString& description)
        : m_id(id), m_name(name), m_theme(theme), m_description(description) {}

    // Getters
    int getId() const { return m_id; }
    const QString& getName() const { return m_name; }
    const QString& getTheme() const { return m_theme; }
    const QString& getDescription() const { return m_description; }

    // Setters
    void setName(const QString& name) { m_name = name; }
    void setTheme(const QString& theme) { m_theme = theme; }
    void setDescription(const QString& desc) { m_description = desc; }

    // Convert to struct
    LocationData toLocationData() const {
        return {m_id, m_name, m_theme, m_description};
    }

private:
    int m_id = -1;
    QString m_name;
    QString m_theme;
    QString m_description;
};
