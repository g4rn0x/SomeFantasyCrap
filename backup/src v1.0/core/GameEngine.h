#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include "GameState.h"
#include "Types.h"

class DatabaseManager;
class Location;
class Item;
class Riddle;
class Note;

/**
 * @brief GameEngine - Main game logic orchestrator
 * Responsible for: game initialization, move processing, state transitions
 * Follows Single Responsibility Principle
 */
class GameEngine : public QObject {
    Q_OBJECT

public:
    explicit GameEngine(QObject* parent = nullptr);
    ~GameEngine();

    // Game lifecycle
    void initializeGame();
    GameState processMove(const GameState& currentState, int doorIndex);
    void handleRiddleAnswer(const GameState& currentState, const QString& answer);

    // State queries
    bool checkWinCondition(const GameState& state) const;
    bool hasGameEnded(const GameState& state) const;

public slots:
    void onDoorSelected(int doorIndex);
    // Signal notification for UI
signals:
    void gameStateChanged(const GameState& newState);
    void errorOccurred(const QString& error);
    void gameInitialized(const GameState& initialState);

private:
    // Helper methods
    QVector<DoorData> generateDoors() const;
    bool processKeyRequirement(GameState& state, const DoorData& door);
    void handleLocationTransition(GameState& state);
    void handleEventGeneration(GameState& state);
    void generateRoomDescription(GameState& state);
    ItemType randomItem() const;

    // Data members
    std::unique_ptr<DatabaseManager> m_database;
    QVector<LocationData> m_locations;
    QVector<RiddleData> m_riddles;
    QVector<NoteData> m_notes;
    GameState m_currentState;
    int m_moveCount = 0;
    int m_movesRemaining;
};
