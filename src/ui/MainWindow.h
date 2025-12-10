#pragma once

#include <QMainWindow>
#include <memory>
#include <QPushButton>

class GameEngine;
class GameWidget;

/**
 * @brief MainWindow - Qt main application window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();



private:
    void setupUI();
    void connectSignals();

    std::unique_ptr<GameEngine> m_engine;
    GameWidget* m_gameWidget = nullptr;
};
