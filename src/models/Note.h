#pragma once

#include <QString>
#include "../core/Types.h"

/**
 * @brief Note - Domain model for game notes/hints
 */
class Note {
public:
    Note() = default;
    Note(int id, const QString& content, int locationId = -1)
        : m_id(id), m_content(content), m_locationId(locationId) {}

    // Getters
    int getId() const { return m_id; }
    const QString& getContent() const { return m_content; }
    int getLocationId() const { return m_locationId; }

    // Setters
    void setContent(const QString& content) { m_content = content; }
    void setLocationId(int id) { m_locationId = id; }

    // Convert to struct
    NoteData toNoteData() const {
        return {m_id, m_content, m_locationId};
    }

private:
    int m_id = -1;
    QString m_content;
    int m_locationId = -1;
};
