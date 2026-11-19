#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

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
    bool m_inputIsInAlternatePanel;
};

#endif // MAINWINDOW_H
