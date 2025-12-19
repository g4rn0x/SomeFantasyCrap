#pragma once

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include "../core/Types.h"

/**
 * @brief InventoryPanel - Панель инвентаря с визуальными иконками
 */
class InventoryPanel : public QWidget {
    Q_OBJECT

public:
    explicit InventoryPanel(QWidget* parent = nullptr);

    /**
     * @brief Обновить отображение инвентаря
     */
    void updateInventory(const QVector<ItemType>& items);

private:
    void setupUI();
    QPixmap loadItemIcon(ItemType type);

    QLabel* m_slots[3];
    QLabel* m_nameLabels[3];
    QPixmap m_goldKeyIcon;
    QPixmap m_silverKeyIcon;
};