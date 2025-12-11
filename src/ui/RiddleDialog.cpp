#include "RiddleDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

RiddleDialog::RiddleDialog(const RiddleData& riddle, QWidget* parent)
    : QDialog(parent), m_riddle(riddle)
{
    setWindowTitle("[Загадка] Встреча со стражником");
    setModal(true);
    setMinimumWidth(500);
    setMinimumHeight(250);

    setupUI();
    connectSignals();
}

void RiddleDialog::setupUI()
{
    setStyleSheet("QDialog { background-color: #222; border: 2px solid #ffff00; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel* headerLabel = new QLabel("[!] ПУТЬ ПРЕГРАЖДАЕТ ЗАГАДОЧНИК", this);
    headerLabel->setStyleSheet("QLabel { color: #ffff00; font-weight: bold; background-color: transparent; }");
    mainLayout->addWidget(headerLabel);

    // Текст загадки
    m_questionLabel = new QLabel(this);
    m_questionLabel->setText(QString("Загадка: %1").arg(m_riddle.question));
    m_questionLabel->setWordWrap(true);
    m_questionLabel->setStyleSheet("QLabel { background-color: #333; color: #fff; padding: 10px; border: 1px solid #ffff00; }");
    mainLayout->addWidget(m_questionLabel);

    // Поле ввода
    QLabel* inputLabel = new QLabel("Ваш ответ:", this);
    inputLabel->setStyleSheet("QLabel { color: #ffffff; font-weight: bold; background-color: transparent; }");
    mainLayout->addWidget(inputLabel);

    m_answerInput = new QLineEdit(this);
    m_answerInput->setPlaceholderText("Введите ответ...");
    m_answerInput->setStyleSheet("QLineEdit { background-color: #333; color: #fff; border: 1px solid #ffff00; padding: 5px; }");
    m_answerInput->setFocus();
    mainLayout->addWidget(m_answerInput);

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_submitButton = new QPushButton("[OK] Ответить", this);
    m_skipButton = new QPushButton("[X] Пропустить", this);

    QString buttonStyle = "QPushButton { background-color: #333; color: #ffff00; border: 1px solid #ffff00; padding: 5px; } QPushButton:hover { background-color: #444; }";
    m_submitButton->setStyleSheet(buttonStyle);
    m_skipButton->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(m_submitButton);
    buttonLayout->addWidget(m_skipButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void RiddleDialog::connectSignals()
{
    connect(m_submitButton, &QPushButton::clicked, this, [this]() {
        m_answer = m_answerInput->text();
        accept();
    });

    connect(m_skipButton, &QPushButton::clicked, this, &QDialog::reject);

    connect(m_answerInput, &QLineEdit::returnPressed, m_submitButton, &QPushButton::click);
}

QString RiddleDialog::getAnswer() const
{
    return m_answer;
}