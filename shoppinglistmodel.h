#ifndef SHOPPINGLISTMODEL_H
#define SHOPPINGLISTMODEL_H

#include <QObject>
#include <QStringList>

// Модель хранит товары и сообщает интерфейсу об изменениях.
class ShoppingListModel : public QObject
{
    Q_OBJECT

public:
    explicit ShoppingListModel(QObject *parent = nullptr);

    QStringList items() const;
    int count() const;

public slots:
    void addItem(const QString &name);
    void removeItem(const QString &name);

signals:
    void itemAdded(const QString &name);
    void itemRemoved(const QString &name);
    void itemCountChanged(int count);
    void errorOccurred(const QString &message);

private:
    QStringList m_items;
};

#endif // SHOPPINGLISTMODEL_H
