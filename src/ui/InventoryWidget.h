#pragma once

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QVector>
#include "../core/Types.h"

QString itemTypeToString(ItemType type);

class InventoryWidget : public QWidget {
    Q_OBJECT

public:
    explicit InventoryWidget(QWidget* parent = nullptr);
    void updateInventory(const QVector<ItemType>& items);

private:
    QListWidget* m_inventoryList;
};