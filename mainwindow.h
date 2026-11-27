#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMetaObject>
#include <QPointer>
#include <QTimer>

class QLineEdit;
class QListWidget;
class QLabel;
class QWidget;
class ShoppingListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addCurrentItem();
    void removeSelectedItem();
    void showError(const QString &message);
    void moveInputField();
    void deleteTemporaryPanel();
    void toggleNotificationTarget();

private:
    void refreshList();

    ShoppingListModel *m_model;
    QWidget *m_inputPanel;
    QWidget *m_alternatePanel;
    QPointer<QWidget> m_temporaryPanel;
    QLineEdit *m_itemEdit;
    QListWidget *m_itemList;
    QLabel *m_countLabel;
    QLabel *m_statusLabel;
    QLabel *m_clockLabel;
    QTimer m_clockTimer;
    QMetaObject::Connection m_itemAddedNotificationConn;
    bool m_inputIsInAlternatePanel;
    bool m_notificationsInTitle;
};

#endif // MAINWINDOW_H
