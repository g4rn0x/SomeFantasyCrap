#pragma once

#include <QWidget>
#include <QLabel>
#include "../core/Types.h"

/**
 * @brief InventoryWidget - Displays player inventory
 */
class InventoryWidget : public QWidget {
    Q_OBJECT

public:
    explicit InventoryWidget(QWidget* parent = nullptr);

    void setItems(const QVector<ItemType>& items);
    void clear();

private:
    void setupUI();
    void updateDisplay();

    QVector<ItemType> m_items;
    QLabel* m_displayLabel = nullptr;
};
