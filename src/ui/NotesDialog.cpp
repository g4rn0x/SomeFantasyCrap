#include "NotesDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QString>
#include <QPushButton>

NotesDialog::NotesDialog(const QVector<NoteData>& notes, QWidget* parent)
    : QDialog(parent), m_notes(notes)
{
    setWindowTitle("[Записки] Найденные послания");
    setModal(true);
    setMinimumSize(600, 400);

    setupUI();
    updateNotesDisplay();
}

void NotesDialog::setupUI()
{
    setStyleSheet("QDialog { background-color: #222; border: 2px solid #00ff00; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel* headerLabel = new QLabel("[?] НАЙДЕННЫЕ ЗАПИСКИ", this);
    headerLabel->setStyleSheet("QLabel { color: #00ff00; font-weight: bold; background-color: transparent; }");
    mainLayout->addWidget(headerLabel);

    // Счетчик
    m_counterLabel = new QLabel(this);
    m_counterLabel->setStyleSheet("QLabel { color: #ffffff; font-weight: bold; background-color: transparent; }");
    mainLayout->addWidget(m_counterLabel);

    // Главный контейнер
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // Список записок слева
    m_notesList = new QListWidget(this);
    m_notesList->setStyleSheet(
        "QListWidget { background-color: #333; color: #00ff00; border: 1px solid #00ff00; }"
        "QListWidget::item:selected { background-color: #00ff00; color: #000; }"
    );
    m_notesList->setMaximumWidth(250);
    contentLayout->addWidget(m_notesList);

    // Текст записки справа
    m_noteContent = new QTextEdit(this);
    m_noteContent->setReadOnly(true);
    m_noteContent->setStyleSheet("QTextEdit { background-color: #333; color: #00ff00; border: 1px solid #00ff00; padding: 10px; font-size: 12px; }");
    contentLayout->addWidget(m_noteContent);

    mainLayout->addLayout(contentLayout);

    // Кнопка закрытия
    QPushButton* closeButton = new QPushButton("[OK] Закрыть", this);
    closeButton->setStyleSheet("QPushButton { background-color: #333; color: #00ff00; border: 1px solid #00ff00; padding: 5px; } QPushButton:hover { background-color: #444; }");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);

    setLayout(mainLayout);

    // Соединяем сигналы
    connect(m_notesList, &QListWidget::itemSelectionChanged, this, [this]() {
        if (m_notesList->currentRow() >= 0) {
            onNoteSelected(m_notesList->currentRow());
        }
    });
}

void NotesDialog::updateNotesDisplay()
{
    m_notesList->clear();

    for (int i = 0; i < m_notes.size(); ++i) {
        QString itemText = QString("Записка #%1").arg(i + 1);
        QListWidgetItem* item = new QListWidgetItem(itemText);
        m_notesList->addItem(item);
    }

    m_counterLabel->setText(
        QString("Записок найдено: %1").arg(m_notes.size())
    );

    // Выбрать первую записку по умолчанию
    if (!m_notes.isEmpty()) {
        m_notesList->setCurrentRow(0);
        onNoteSelected(0);
    }
}

void NotesDialog::onNoteSelected(int index)
{
    if (index >= 0 && index < m_notes.size()) {
        m_noteContent->setText(m_notes[index].content);
    }
}