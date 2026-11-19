#include "mainwindow.h"
#include "shoppinglistmodel.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

// Небольшие классы нужны, чтобы видеть создание и уничтожение виджетов.
class TrackedWidget : public QWidget
{
public:
    explicit TrackedWidget(const QString &objectName, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setObjectName(objectName);
        qDebug() << objectName << "created";
    }

    ~TrackedWidget() override
    {
        qDebug() << objectName() << "destroyed";
    }
};

class TrackedLineEdit : public QLineEdit
{
public:
    explicit TrackedLineEdit(QWidget *parent = nullptr) : QLineEdit(parent)
    {
        qDebug() << "Item input created";
    }

    ~TrackedLineEdit() override
    {
        qDebug() << "Item input destroyed";
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_model(new ShoppingListModel(this)),
      m_inputPanel(nullptr),
      m_alternatePanel(nullptr),
      m_itemEdit(nullptr),
      m_itemList(nullptr),
      m_countLabel(nullptr),
      m_statusLabel(nullptr),
      m_inputIsInAlternatePanel(false)
{
    qDebug() << "MainWindow created";
    setWindowTitle("Список покупок");
    resize(520, 500);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *title = new QLabel("Мой список покупок", central);
    mainLayout->addWidget(title);

    m_inputPanel = new TrackedWidget("InputPanel", central);
    QHBoxLayout *inputLayout = new QHBoxLayout(m_inputPanel);
    m_itemEdit = new TrackedLineEdit(m_inputPanel);
    m_itemEdit->setPlaceholderText("Название товара");
    QPushButton *addButton = new QPushButton("Добавить", m_inputPanel);
    inputLayout->addWidget(m_itemEdit);
    inputLayout->addWidget(addButton);
    mainLayout->addWidget(m_inputPanel);

    m_alternatePanel = new TrackedWidget("AlternatePanel", central);
    QHBoxLayout *alternateLayout = new QHBoxLayout(m_alternatePanel);
    QLabel *alternateLabel = new QLabel("Дополнительная область:", m_alternatePanel);
    alternateLayout->addWidget(alternateLabel);
    mainLayout->addWidget(m_alternatePanel);

    m_itemList = new QListWidget(central);
    mainLayout->addWidget(m_itemList);

    QHBoxLayout *actionsLayout = new QHBoxLayout;
    QPushButton *removeButton = new QPushButton("Удалить выбранный", central);
    QPushButton *moveButton = new QPushButton("Переместить поле", central);
    actionsLayout->addWidget(removeButton);
    actionsLayout->addWidget(moveButton);
    mainLayout->addLayout(actionsLayout);

    m_countLabel = new QLabel("Товаров: 0", central);
    m_statusLabel = new QLabel("Готово", central);
    mainLayout->addWidget(m_countLabel);
    mainLayout->addWidget(m_statusLabel);

    // Временная панель нужна для демонстрации deleteLater() и QPointer.
    m_temporaryPanel = new TrackedWidget("TemporaryPanel", central);
    QHBoxLayout *temporaryLayout = new QHBoxLayout(m_temporaryPanel);
    temporaryLayout->addWidget(new QLabel("Временная панель", m_temporaryPanel));
    QPushButton *deletePanelButton = new QPushButton("Удалить временную панель", central);
    mainLayout->addWidget(m_temporaryPanel);
    mainLayout->addWidget(deletePanelButton);

    connect(addButton, &QPushButton::clicked,
            this, &MainWindow::addCurrentItem);
    connect(m_itemEdit, &QLineEdit::returnPressed,
            this, &MainWindow::addCurrentItem);
    connect(removeButton, &QPushButton::clicked,
            this, &MainWindow::removeSelectedItem);
    connect(moveButton, &QPushButton::clicked,
            this, &MainWindow::moveInputField);
    connect(deletePanelButton, &QPushButton::clicked,
            this, &MainWindow::deleteTemporaryPanel);

    // Один сигнал модели подключён к нескольким обработчикам.
    connect(m_model, &ShoppingListModel::itemAdded,
            this, [this](const QString &) { refreshList(); });
    connect(m_model, &ShoppingListModel::itemAdded,
            this, [](const QString &name) {
                qDebug() << "Событие добавления товара:" << name;
            });
    connect(m_model, &ShoppingListModel::itemRemoved,
            this, [this](const QString &) { refreshList(); });
    connect(m_model, &ShoppingListModel::itemCountChanged,
            this, [this](int count) {
                m_countLabel->setText(QString("Товаров: %1").arg(count));
            });
    connect(m_model, &ShoppingListModel::errorOccurred,
            this, &MainWindow::showError);
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destroyed";
}

void MainWindow::addCurrentItem()
{
    m_model->addItem(m_itemEdit->text());
    m_itemEdit->clear();
    m_itemEdit->setFocus();
}

void MainWindow::removeSelectedItem()
{
    if (!m_itemList->currentItem()) {
        showError("Сначала выберите товар в списке.");
        return;
    }
    m_model->removeItem(m_itemList->currentItem()->text());
}

void MainWindow::showError(const QString &message)
{
    m_statusLabel->setText("Ошибка: " + message);
    qDebug() << "Ошибка:" << message;
}

void MainWindow::refreshList()
{
    m_itemList->clear();
    m_itemList->addItems(m_model->items());
    m_statusLabel->setText("Список обновлён");
}

void MainWindow::moveInputField()
{
    QHBoxLayout *oldLayout = m_inputIsInAlternatePanel
            ? qobject_cast<QHBoxLayout *>(m_alternatePanel->layout())
            : qobject_cast<QHBoxLayout *>(m_inputPanel->layout());
    QWidget *newParent = m_inputIsInAlternatePanel ? m_inputPanel : m_alternatePanel;
    QHBoxLayout *newLayout = qobject_cast<QHBoxLayout *>(newParent->layout());

    oldLayout->removeWidget(m_itemEdit);
    m_itemEdit->setParent(newParent);
    newLayout->addWidget(m_itemEdit);
    m_itemEdit->show();

    m_inputIsInAlternatePanel = !m_inputIsInAlternatePanel;
    qDebug() << "Родитель поля ввода изменён на:" << newParent->objectName();
}

void MainWindow::deleteTemporaryPanel()
{
    if (m_temporaryPanel) {
        qDebug() << "TemporaryPanel scheduled for deletion";
        m_temporaryPanel->deleteLater();
    } else {
        m_statusLabel->setText("Временная панель уже удалена");
    }
}
