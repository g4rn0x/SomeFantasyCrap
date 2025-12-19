#include "InventoryWidget.h"
#include <QString>
#include <QIcon>

InventoryWidget::InventoryWidget(QWidget* parent)
    : QWidget(parent)
    , m_inventoryList(new QListWidget(this))
{
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_inventoryList);
    setLayout(layout);

    // Basic styling
    m_inventoryList->setIconSize(QSize(32, 32));
    setMinimumWidth(200);
}

void InventoryWidget::updateInventory(const QVector<ItemType>& items) {
    m_inventoryList->clear();

    for (const auto& item : items) {
        auto* itemWidget = new QListWidgetItem(itemTypeToString(item), m_inventoryList);
        m_inventoryList->addItem(itemWidget);
    }
}