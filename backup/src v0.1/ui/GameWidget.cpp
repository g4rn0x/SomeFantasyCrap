#include "GameWidget.h"
#include "../core/GameEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QTextEdit>

GameWidget::GameWidget(GameEngine* engine, QWidget* parent)
    : QWidget(parent), m_engine(engine)
{
    setupUI();
}

void GameWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Room description
    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setStyleSheet("QLabel { background-color: #2a2a2a; color: #00ff00; padding: 15px; font-size: 14px; border-radius: 5px; }");
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setMinimumHeight(100);
    mainLayout->addWidget(m_descriptionLabel);
    
    // Status bar
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("QLabel { background-color: #1a1a1a; color: #ffff00; padding: 10px; font-weight: bold; }");
    mainLayout->addWidget(m_statusLabel);
    
    // Inventory
    m_inventoryLabel = new QLabel(this);
    m_inventoryLabel->setStyleSheet("QLabel { background-color: #1a1a1a; color: #00aaff; padding: 10px; }");
    mainLayout->addWidget(m_inventoryLabel);
    
    // Doors section
    QLabel* doorsLabel = new QLabel("Выберите дверь:", this);
    doorsLabel->setStyleSheet("QLabel { color: #ffffff; font-weight: bold; font-size: 16px; padding: 10px; }");
    mainLayout->addWidget(doorsLabel);
    
    m_doorsLayout = new QVBoxLayout();
    mainLayout->addLayout(m_doorsLayout);
    
    // Stretch to fill
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void GameWidget::onGameInitialized(const GameState& state)
{
    updateDisplay(state);
}

void GameWidget::onGameStateChanged(const GameState& state)
{
    updateDisplay(state);
}

void GameWidget::onErrorOccurred(const QString& error)
{
    m_statusLabel->setText(QString("❌ ОШИБКА: %1").arg(error));
}

void GameWidget::updateDisplay(const GameState& state)
{
    // Update description
    m_descriptionLabel->setText(state.getRoomDescription());
    
    // Update status
    QString status = QString(
        "📍 Локация: %1/5 | 🚪 Комната: %2/10 | 💰 Золото: %3"
    ).arg(state.getCurrentLocationIndex() + 1, 
          state.getCurrentRoomIndex() + 1, 
          state.getGoldBars());
    m_statusLabel->setText(status);
    
    // Update inventory
    QString inventory = "📦 Инвентарь (";
    for (ItemType item : state.getInventory()) {
        inventory += itemTypeToString(item) + "; ";
    }
    inventory += QString("%1/%2)").arg(state.getInventory().size(), MAX_INVENTORY_SIZE);
    m_inventoryLabel->setText(inventory);
    
    // Update door buttons
    updateDoorButtons(state);
    
    // Check win/lose conditions
    if (state.isGameWon()) {
        m_statusLabel->setText("🏆 ПОБЕДА! Вы прошли все локации!");
    }
}

void GameWidget::updateDoorButtons(const GameState& state)
{
    // Clear old buttons
    for (QPushButton* btn : m_doorButtons) {
        delete btn;
    }
    m_doorButtons.clear();
    
    // Create new buttons for current doors
    int doorIndex = 0;
    for (const DoorData& door : state.getCurrentDoors()) {
        QPushButton* button = new QPushButton(
            QString("Дверь %1: %2").arg(doorIndex + 1, doorTypeToString(door.type)),
            this
        );
        
        // Color by type
        QString style = "QPushButton { padding: 10px; font-size: 14px; font-weight: bold; ";
        if (door.type == DoorType::NORMAL) {
            style += "background-color: #4a4a4a; color: #ffffff; ";
        } else if (door.type == DoorType::SILVER) {
            style += "background-color: #c0c0c0; color: #000000; ";
        } else { // GOLD
            style += "background-color: #ffd700; color: #000000; ";
        }
        style += "border-radius: 5px; }"
                 "QPushButton:hover { opacity: 0.8; }";
        button->setStyleSheet(style);
        button->setMinimumHeight(40);
        
        int currentIndex = doorIndex;
        connect(button, &QPushButton::clicked, this, [this, currentIndex]() {
            onDoorClicked(currentIndex);
        });
        
        m_doorsLayout->addWidget(button);
        m_doorButtons.append(button);
        doorIndex++;
    }
}

void GameWidget::onDoorClicked(int doorIndex)
{
    if (!m_engine) return;
    // This would be called by the game engine through the signal
    // The actual move processing is handled by GameEngine
}
