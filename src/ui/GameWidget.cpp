#include "GameWidget.h"
#include "../core/GameEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QTextEdit>
#include "InventoryPanel.h"
#include "NotesDialog.h"
#include "RiddleDialog.h"
#include <QMessageBox>
#include <QApplication>

GameWidget::GameWidget(GameEngine* engine, QWidget* parent)
    : QWidget(parent), m_engine(engine) {
    setupUI();
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(true);
}

void GameWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    // Обновить фон при изменении размера
    if (!m_currentState.getLocationImagePath().isEmpty()) {
        QPixmap bg(m_currentState.getLocationImagePath());
        if (!bg.isNull()) {
            QPalette palette;
            palette.setBrush(this->backgroundRole(), QBrush(bg.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            this->setPalette(palette);
        }
    }
}

void GameWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // Description
    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0.5); color: #00ff00; padding: 15px; border: 2px solid #00ff00; font-size: 18px; }");
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setMinimumHeight(180);
    mainLayout->addWidget(m_descriptionLabel);

    m_typeWriterLabel = new QLabel(this);
    m_typeWriterLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0.5); color: #00ff00; padding: 15px; border: 2px solid #00ff00; font-size: 18px; }");
    m_typeWriterLabel->setWordWrap(true);
    m_typeWriterLabel->setMinimumHeight(180);
    m_typeWriterLabel->setVisible(false);
    mainLayout->addWidget(m_typeWriterLabel);

    // Sidebar
    QHBoxLayout* sidebarLayout = new QHBoxLayout();
    m_notesButton = new QPushButton("Записки", this);
    m_notesButton->setStyleSheet("QPushButton { background-color: rgba(0,0,0,0.5); color: #fff; border: 2px solid #fff; padding: 10px; } QPushButton:hover { background-color: #fff; color: #000; }");
    m_inventoryPanel = new InventoryPanel(this);
    sidebarLayout->addWidget(m_notesButton);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(m_inventoryPanel);
    mainLayout->addLayout(sidebarLayout);

    // Doors
    m_doorsLayout = new QHBoxLayout();
    mainLayout->addLayout(m_doorsLayout);

    mainLayout->addStretch();

    // Status, Log, Exit
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0.5); color: #ffff00; padding: 10px; border: 2px solid #ffff00; }");
    mainLayout->addWidget(m_statusLabel);

    m_logView = new QTextEdit(this);
    m_logView->setStyleSheet("QTextEdit { background-color: rgba(0, 0, 0, 0.5); color: #00ff00; border: 2px solid #00ff00; }");
    m_logView->setReadOnly(true);
    m_logView->setMaximumHeight(100);
    mainLayout->addWidget(m_logView);

    QHBoxLayout* exitLayout = new QHBoxLayout();
    m_exitButton = new QPushButton("Выход", this);
    m_exitButton->setFixedWidth(150);
    m_exitButton->setStyleSheet("QPushButton { background-color: rgba(0,0,0,0.5); color: #ff0000; border: 2px solid #ff0000; padding: 10px; } QPushButton:hover { background-color: #ff0000; color: #fff; }");
    connect(m_exitButton, &QPushButton::clicked, this, &GameWidget::onExitButtonClicked);
    connect(m_notesButton, &QPushButton::clicked, this, &GameWidget::onNotesButtonClicked);
    exitLayout->addStretch();
    exitLayout->addWidget(m_exitButton);
    exitLayout->addStretch();
    mainLayout->addLayout(exitLayout);

    setLayout(mainLayout);
}

void GameWidget::onGameInitialized(const GameState& state)
{
    updateDisplay(state);
}

void GameWidget::onGameStateChanged(const GameState& state)
{
    qDebug() << "=== onGameStateChanged called ===";
    qDebug() << "Logs count: " << state.getLogs().size();
    updateDisplay(state);
}

void GameWidget::onErrorOccurred(const QString& error)
{
    m_statusLabel->setText(QString("❌ ОШИБКА: %1").arg(error));
}

void GameWidget::updateDisplay(const GameState& state)
{
    m_currentState = state;
    if (!state.isTypeWriterActive()) {
        m_descriptionLabel->setText(state.getRoomDescription());
    }

    // Update status
    QString status = QString("Локация: %1/5 | Комната: %2/10 | Золото: %3")
        .arg(state.getCurrentLocationIndex() + 1)
        .arg(state.getCurrentRoomIndex() + 1)
        .arg(state.getGoldBars());
    m_statusLabel->setText(status);

    // Update inventory
    m_inventoryPanel->updateInventory(state.getInventory());

    // Update door buttons
    updateDoorButtons(state);

    // Update background
    if (!state.getLocationImagePath().isEmpty()) {
        QPixmap bg(state.getLocationImagePath());
        if (!bg.isNull()) {
            QPalette palette;
            palette.setBrush(this->backgroundRole(), QBrush(bg.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            this->setPalette(palette);
        }
    }

    // Check win/lose conditions
    if (state.isGameWon()) {
        m_statusLabel->setText(" ПОБЕДА! Вы прошли все локации!");
    }

    // Update logs
    m_logView->clear();
    for (const QString& log : state.getLogs()) {
        m_logView->append(log);
    }
}

void GameWidget::updateDoorButtons(const GameState& state)
{
    // Clear old buttons and layout
    for (QPushButton* btn : m_doorButtons) {
        delete btn;
    }
    m_doorButtons.clear();
    QLayoutItem* item;
    while ((item = m_doorsLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    // Create new buttons for current doors
    int doorIndex = 0;
    for (const DoorData& door : state.getCurrentDoors()) {
        QPushButton* button = new QPushButton(
            QString("Дверь %1: %2").arg(doorIndex + 1).arg(doorTypeToString(door.type)),
            this
        );

        QString style;
        if (door.type == DoorType::NORMAL) {
            style = "QPushButton { background-color: rgba(0,0,0,0.5); color: #8B4513; border: 2px solid #8B4513; padding: 15px; font-size: 18px; } QPushButton:hover { background-color: #8B4513; color: #fff; }";
        } else if (door.type == DoorType::SILVER) {
            style = "QPushButton { background-color: rgba(0,0,0,0.5); color: #c0c0c0; border: 2px solid #c0c0c0; padding: 15px; font-size: 18px; } QPushButton:hover { background-color: #c0c0c0; color: #000; }";
        } else { // GOLD
            style = "QPushButton { background-color: rgba(0,0,0,0.5); color: #ffd700; border: 2px solid #ffd700; padding: 15px; font-size: 18px; } QPushButton:hover { background-color: #ffd700; color: #000; }";
        }
        button->setStyleSheet(style);
        button->setMinimumHeight(40);

        int currentIndex = doorIndex;
        connect(button, &QPushButton::clicked, this, [this, currentIndex]() {
            onDoorClicked(currentIndex);
        });

        m_doorButtons.append(button);
        doorIndex++;
    }

    // Add stretchable space to center the buttons
    m_doorsLayout->addStretch();
    for (QPushButton* button : m_doorButtons) {
        m_doorsLayout->addWidget(button);
    }
    m_doorsLayout->addStretch();
}

void GameWidget::onDoorClicked(int doorIndex)
{
    if (!m_engine) return;
    emit doorSelected(doorIndex);
}

void GameWidget::onNoteFound(const NoteData& note)
{
    m_foundNotes.append(note);
    m_currentNotesFound++;
    m_notesButton->setText(QString("Записки (%1)").arg(m_currentNotesFound));
}

void GameWidget::onRiddleEncountered(const RiddleData& riddle)
{
    qDebug() << "=== onRiddleEncountered called ===";
    qDebug() << "Question:" << riddle.question;
    qDebug() << "Correct answer:" << riddle.answer;

    RiddleDialog* dialog = new RiddleDialog(riddle, this);
    connect(dialog, &RiddleDialog::finished, this, &GameWidget::onRiddleDialogFinished);
    dialog->open();
}


void GameWidget::onRiddleDialogFinished(int result)
{
    RiddleDialog* dialog = qobject_cast<RiddleDialog*>(sender());
    if (!dialog) return;

    if (result == QDialog::Accepted) {
        emit riddleAnswered(dialog->getAnswer());
    } else {
        emit riddleAnswered("");
    }

    dialog->deleteLater();
}

void GameWidget::onNotesButtonClicked()
{
    NotesDialog dialog(m_foundNotes, this);
    dialog.exec();
}

void GameWidget::onExitButtonClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти?",
                                QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void GameWidget::onTypeWriterStarted(const QString& text)
{
    m_typeWriterLabel->setText("");
    m_descriptionLabel->setVisible(false);
    m_typeWriterLabel->setVisible(true);
}

void GameWidget::onTypeWriterFinished()
{
    m_descriptionLabel->setVisible(true);
    m_typeWriterLabel->setVisible(false);
}

void GameWidget::onRoomDescriptionUpdated(const QString& text)
{
    m_typeWriterLabel->setText(text);
}

void GameWidget::onGameWon(int notesFound, int goldBars)
{
    // Disable all doors
    for (QPushButton* btn : m_doorButtons) {
        btn->setEnabled(false);
    }

    QString winMessage = QString(
        "<div style='text-align: center;'>"
        "<h1>ПОБЕДА!</h1>"
        "<p>Вы успешно выбрались из лабиринта!</p>"
        "<hr>"
        "<h2>Ваша статистика:</h2>"
        "<p><b>Записок найдено:</b> %1</p>"
        "<p><b>Золотых слитков:</b> %2</p>"
        "</div>"
    ).arg(notesFound).arg(goldBars);

    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle("Игра окончена");
    msgBox->setTextFormat(Qt::RichText);
    msgBox->setText(winMessage);
    msgBox->setStandardButtons(QMessageBox::Ok);
    connect(msgBox, &QMessageBox::finished, qApp, &QApplication::quit);
    msgBox->open();
}

void GameWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space) {
        m_engine->skipCurrentTypeWriter();
    }
    QWidget::keyPressEvent(event);
}