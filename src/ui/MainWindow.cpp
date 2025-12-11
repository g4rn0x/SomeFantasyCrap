#include "MainWindow.h"
#include "GameWidget.h"
#include "../core/GameEngine.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_engine(std::make_unique<GameEngine>()),
      m_gameWidget(new GameWidget(m_engine.get(), this))
{
    setupUI();
    connectSignals();
    
    setWindowTitle("🎮 Лабиринт");
    resize(1200, 800);
    
    // Initialize game
    QTimer::singleShot(0, m_engine.get(), &GameEngine::initializeGame);

    // Set focus to the game widget
    m_gameWidget->setFocus();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_gameWidget);
    
    setCentralWidget(centralWidget);
}


void MainWindow::connectSignals()
{
    // Connect engine signals to game widget
    connect(m_engine.get(), &GameEngine::gameInitialized,
            m_gameWidget, &GameWidget::onGameInitialized);
    connect(m_engine.get(), &GameEngine::gameStateChanged,
            m_gameWidget, &GameWidget::onGameStateChanged);
    connect(m_engine.get(), &GameEngine::errorOccurred,
            m_gameWidget, &GameWidget::onErrorOccurred);
    connect(m_gameWidget, &GameWidget::doorSelected,
            m_engine.get(), &GameEngine::onDoorSelected);
    connect(m_engine.get(), &GameEngine::noteFound,
            m_gameWidget, &GameWidget::onNoteFound);
    connect(m_engine.get(), &GameEngine::riddleEncountered,
            m_gameWidget, &GameWidget::onRiddleEncountered);
    connect(m_gameWidget, &GameWidget::riddleAnswered,
            m_engine.get(), &GameEngine::handleRiddleAnswer);
    connect(m_engine.get(), &GameEngine::gameWon,
            m_gameWidget, &GameWidget::onGameWon);

    // Typewriter connections
    connect(m_engine.get(), &GameEngine::typeWriterStarted,
            m_gameWidget, &GameWidget::onTypeWriterStarted);
    connect(m_engine.get(), &GameEngine::roomDescriptionUpdated,
            m_gameWidget, &GameWidget::onRoomDescriptionUpdated);
    connect(m_engine.get(), &GameEngine::typeWriterFinished,
            m_gameWidget, &GameWidget::onTypeWriterFinished);
}

