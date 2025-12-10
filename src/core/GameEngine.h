#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include "GameState.h"
#include "Types.h"
#include "../utils/TypeWriter.h"


class DatabaseManager;
class RiggleDIalog;
class NotesDialog;
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

    void handleRiddleAnswer(const QString& answer);
    void addFoundNote(const NoteData& note);
    int getTotalNotesFound() const { return m_totalNotesFound; }

    void startRoomDescriptionTypeWriter(const QString& text);
    void skipCurrentTypeWriter();

    GameState processMove(const GameState& currentState, int doorIndex);

    // State queries
    bool checkWinCondition(const GameState& state) const;
    bool hasGameEnded(const GameState& state) const;

    QString getGeneratedRoomDescription(int locationId, int roomNumber);

    const GameState& getCurrentState() const { return m_currentState; }

public slots:
    void onDoorSelected(int doorIndex);
    // Signal notification for UI
signals:
    void gameStateChanged(const GameState& newState);
    void errorOccurred(const QString& error);
    void gameInitialized(const GameState& initialState);
    void typeWriterStarted(const QString& text);
    void typeWriterFinished();
    void roomDescriptionUpdated(const QString& description);
    void noteFound(const NoteData& note);
    void riddleEncountered(const RiddleData& riddle);

private:
    // Helper methods
    QVector<DoorData> generateDoors() const;
    bool processKeyRequirement(GameState& state, const DoorData& door);
    void handleLocationTransition(GameState& state);
    void handleEventGeneration(GameState& state, const DoorData& door);
    void generateRoomDescription(GameState& state);
    ItemType randomItem(bool isSilverDoor = false) const;

    void onTypeWriterCharacterAdded(const QString& text);
    void onTypeWriterFinished();

    // Data members
    std::unique_ptr<DatabaseManager> m_database;
    std::unique_ptr<TypeWriter> m_typeWriter;
    std::shared_ptr<RiddleData> m_currentRiddle;
    QVector<LocationData> m_locations;
    QVector<RiddleData> m_riddles;
    QVector<NoteData> m_notes;
    GameState m_currentState;
    int m_moveCount = 0;
    int m_movesRemaining;
    int m_totalNotesFound = 0;  // НОВЫЙ СЧЕТЧИК
};
