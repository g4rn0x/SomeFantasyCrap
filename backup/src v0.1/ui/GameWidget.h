#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "../core/GameState.h"

class GameEngine;

/**
 * @brief GameWidget - Main game display widget
 */
class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(GameEngine* engine, QWidget* parent = nullptr);

public slots:
    void onGameInitialized(const GameState& state);
    void onGameStateChanged(const GameState& state);
    void onErrorOccurred(const QString& error);

private:
    void setupUI();
    void updateDisplay(const GameState& state);
    void updateDoorButtons(const GameState& state);
    void onDoorClicked(int doorIndex);

    GameEngine* m_engine = nullptr;
    QLabel* m_descriptionLabel = nullptr;
    QLabel* m_statusLabel = nullptr;
    QLabel* m_inventoryLabel = nullptr;
    QVBoxLayout* m_doorsLayout = nullptr;
    QVector<QPushButton*> m_doorButtons;
};
