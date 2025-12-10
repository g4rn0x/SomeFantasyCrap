#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "../core/Types.h"

/**
 * @brief RiddleDialog - Диалоговое окно для ввода ответа на загадку
 */
class RiddleDialog : public QDialog {
    Q_OBJECT

public:
    explicit RiddleDialog(const RiddleData& riddle, QWidget* parent = nullptr);

    /**
     * @brief Получить введенный ответ
     */
    QString getAnswer() const;

    /**
     * @brief Получить данные загадки
     */
    const RiddleData& getRiddle() const { return m_riddle; }

private:
    void setupUI();
    void connectSignals();

    RiddleData m_riddle;
    QLabel* m_questionLabel = nullptr;
    QLineEdit* m_answerInput = nullptr;
    QPushButton* m_submitButton = nullptr;
    QPushButton* m_skipButton = nullptr;
    QString m_answer;
};