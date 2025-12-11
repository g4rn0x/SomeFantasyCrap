#pragma once

#include <QString>
#include "../core/Types.h"

/**
 * @brief Riddle - Domain model for game riddles
 */
class Riddle {
public:
    Riddle() = default;
    Riddle(int id, const QString& question, const QString& answer, int difficulty = 1)
        : m_id(id), m_question(question), m_answer(answer), m_difficulty(difficulty) {}

    // Getters
    int getId() const { return m_id; }
    const QString& getQuestion() const { return m_question; }
    const QString& getAnswer() const { return m_answer; }
    int getDifficulty() const { return m_difficulty; }

    // Setters
    void setQuestion(const QString& q) { m_question = q; }
    void setAnswer(const QString& a) { m_answer = a; }
    void setDifficulty(int diff) { m_difficulty = diff; }

    // Convert to struct
    RiddleData toRiddleData() const {
        return {m_id, m_question, m_answer, m_difficulty};
    }

private:
    int m_id = -1;
    QString m_question;
    QString m_answer;
    int m_difficulty = 1;
};
