#pragma once

#include <QString>
#include <QVector>
#include <QMetaType>
#include "Types.h"
#include "Constants.h"
/**
 * @brief GameState - Immutable representation of game state at any moment
 * Follows immutability pattern for predictable state management
 */
// Forward declare the GameState class
class GameState {
public:
    // Default constructor
    GameState() = default;
    
    // Getters
    int getCurrentLocationIndex() const { return m_currentLocationIndex; }
    int getCurrentRoomIndex() const { return m_currentRoomIndex; }
    int getGoldBars() const { return m_goldBars; }
    const QVector<ItemType>& getInventory() const { return m_inventory; }
    const QVector<NoteData>& getNotes() const { return m_notes; }
    const QVector<QString>& getLogs() const { return m_logs; }
    bool isGameOver() const { return m_isGameOver; }
    bool isGameWon() const { return m_gameWon; }
    const RiddleData* getActiveRiddle() const { return m_activeRiddle.get(); }
    const QVector<DoorData>& getCurrentDoors() const { return m_doors; }
    const QString& getRoomDescription() const { return m_roomDescription; }
    bool isLoading() const { return m_isLoading; }

    // Setters (Builder pattern)
    GameState& setCurrentLocationIndex(int index) { m_currentLocationIndex = index; return *this; }
    GameState& setCurrentRoomIndex(int index) { m_currentRoomIndex = index; return *this; }
    GameState& setGoldBars(int bars) { m_goldBars = bars; return *this; }
    GameState& setInventory(const QVector<ItemType>& inv) { m_inventory = inv; return *this; }
    GameState& setNotes(const QVector<NoteData>& notes) { m_notes = notes; return *this; }
    GameState& setLogs(const QVector<QString>& logs) { m_logs = logs; return *this; }
    GameState& setGameOver(bool value) { m_isGameOver = value; return *this; }
    GameState& setGameWon(bool value) { m_gameWon = value; return *this; }
    GameState& setActiveRiddle(std::shared_ptr<RiddleData> riddle) { m_activeRiddle = riddle; return *this; }
    GameState& setCurrentDoors(const QVector<DoorData>& doors) { m_doors = doors; return *this; }
    GameState& setRoomDescription(const QString& desc) { m_roomDescription = desc; return *this; }
    GameState& setLoading(bool value) { m_isLoading = value; return *this; }

    // Utility methods
    bool hasInventorySpace() const { return m_inventory.size() < MAX_INVENTORY_SIZE; }
    void addLog(const QString& message) { m_logs.append(message); }
    void addItem(ItemType item) { if (hasInventorySpace()) m_inventory.append(item); }
    bool hasItem(ItemType item) const { return m_inventory.contains(item); }
    void removeItem(ItemType item) { m_inventory.removeOne(item); }
    void addNote(const NoteData& note) { m_notes.append(note); }

private:
    int m_currentLocationIndex = 0;
    int m_currentRoomIndex = 0;
    int m_goldBars = 0;
    QVector<ItemType> m_inventory;
    QVector<NoteData> m_notes;
    QVector<QString> m_logs;
    bool m_isGameOver = false;
    bool m_gameWon = false;
    std::shared_ptr<RiddleData> m_activeRiddle = nullptr;
    QVector<DoorData> m_doors;
    QString m_roomDescription;
    bool m_isLoading = false;
};

