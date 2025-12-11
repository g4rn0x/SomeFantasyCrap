#include "InventoryPanel.h"
#include <QHBoxLayout>
#include <QPainter>

InventoryPanel::InventoryPanel(QWidget* parent)
    : QWidget(parent)
    , m_goldKeyIcon("assets/inventory/gold_key.png")
    , m_silverKeyIcon("assets/inventory/silver_key.png")
{
    setupUI();
}

void InventoryPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* slotsLayout = new QHBoxLayout();
    QHBoxLayout* namesLayout = new QHBoxLayout();
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // Create inventory slots
    for (int i = 0; i < 3; ++i) {
        m_slots[i] = new QLabel(this);
        m_slots[i]->setFixedSize(32, 32);
        m_slots[i]->setStyleSheet("QLabel { background-color: rgba(0,0,0,0.5); border: 2px solid #fff; }");
        slotsLayout->addWidget(m_slots[i]);

        m_nameLabels[i] = new QLabel(this);
        m_nameLabels[i]->setAlignment(Qt::AlignCenter);
        m_nameLabels[i]->setStyleSheet("color: white;");
        namesLayout->addWidget(m_nameLabels[i]);
    }

    // Set initial empty state
    mainLayout->addLayout(slotsLayout);
    mainLayout->addLayout(namesLayout);
    updateInventory({});
}

void InventoryPanel::updateInventory(const QVector<ItemType>& items)
{
    // Clear all slots first
    for (int i = 0; i < 3; ++i) {
        m_slots[i]->clear();
        m_slots[i]->setToolTip("");
        m_nameLabels[i]->clear();
    }

    // Update with current items
    for (int i = 0; i < qMin(items.size(), 3); ++i) {
        QPixmap icon = loadItemIcon(items[i]);
        m_slots[i]->setPixmap(icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        // Set tooltip and name label
        switch (items[i]) {
            case ItemType::GOLD_KEY:
                m_slots[i]->setToolTip("Золотой ключ");
                m_nameLabels[i]->setText("Gold Key");
                break;
            case ItemType::SILVER_KEY:
                m_slots[i]->setToolTip("Серебряный ключ");
                m_nameLabels[i]->setText("Silver Key");
                break;
        }
    }
}

QPixmap InventoryPanel::loadItemIcon(ItemType type)
{
    switch (type) {
        case ItemType::GOLD_KEY:
            return m_goldKeyIcon.isNull() ? QPixmap(30, 30) : m_goldKeyIcon;
        case ItemType::SILVER_KEY:
            return m_silverKeyIcon.isNull() ? QPixmap(30, 30) : m_silverKeyIcon;
        default:
            return QPixmap(30, 30);
    }
}
