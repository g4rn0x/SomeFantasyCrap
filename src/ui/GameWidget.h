#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTextEdit>
#include "../core/GameState.h"

class GameEngine;
class InventoryPanel;

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(GameEngine* engine, QWidget* parent = nullptr);

public slots:
    void onGameInitialized(const GameState& state);
    void onGameStateChanged(const GameState& state);
    void onErrorOccurred(const QString& error);
    void onNoteFound(const NoteData& note);
    void onRiddleEncountered(const RiddleData& riddle);
    void onTypeWriterStarted(const QString& text);
    void onTypeWriterFinished();
    void onRoomDescriptionUpdated(const QString& text);
    void onGameWon(int notesFound, int goldBars);
signals:
    void doorSelected(int doorIndex);
    void riddleAnswered(const QString& answer);
private slots:
    void onRiddleDialogFinished(int result);
private:
    GameState m_currentState;
    void setupUI();
    void updateDisplay(const GameState& state);
    void updateDoorButtons(const GameState& state);
    void onDoorClicked(int doorIndex);

    void onNotesButtonClicked();
    void onExitButtonClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    GameEngine* m_engine = nullptr;
    QLabel* m_descriptionLabel = nullptr;
    QLabel* m_statusLabel = nullptr;
    InventoryPanel* m_inventoryPanel = nullptr;
    QHBoxLayout* m_doorsLayout = nullptr;
    QVector<QPushButton*> m_doorButtons;

    QLabel* m_typeWriterLabel = nullptr;
    QPushButton* m_notesButton = nullptr;
    QPushButton* m_exitButton = nullptr;
    QLabel* m_notesCounterLabel = nullptr;

    int m_currentNotesFound = 0;
    QVector<NoteData> m_foundNotes;
    QTextEdit* m_logView = nullptr;

};
