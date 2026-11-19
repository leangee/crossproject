#include "shoppinglistmodel.h"

#include <QDebug>

ShoppingListModel::ShoppingListModel(QObject *parent)
    : QObject(parent)
{
    qDebug() << "ShoppingListModel created";
}

QStringList ShoppingListModel::items() const
{
    return m_items;
}

int ShoppingListModel::count() const
{
    return m_items.size();
}

void ShoppingListModel::addItem(const QString &name)
{
    const QString cleanedName = name.trimmed();

    if (cleanedName.isEmpty()) {
        emit errorOccurred("Введите название товара.");
        return;
    }

    for (const QString &item : m_items) {
        if (item.compare(cleanedName, Qt::CaseInsensitive) == 0) {
            emit errorOccurred("Такой товар уже есть в списке.");
            return;
        }
    }

    m_items.append(cleanedName);
    qDebug() << "Товар добавлен:" << cleanedName;
    emit itemAdded(cleanedName);
    emit itemCountChanged(m_items.size());
}

void ShoppingListModel::removeItem(const QString &name)
{
    const int index = m_items.indexOf(name);
    if (index < 0) {
        emit errorOccurred("Выберите товар для удаления.");
        return;
    }

    const QString removedName = m_items.takeAt(index);
    qDebug() << "Товар удалён:" << removedName;
    emit itemRemoved(removedName);
    emit itemCountChanged(m_items.size());
}
