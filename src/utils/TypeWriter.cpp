#include "TypeWriter.h"
#include <QTimer>

TypeWriter::TypeWriter(QObject* parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &TypeWriter::onTimerTick);
}

TypeWriter::~TypeWriter()
{
    stop();
}

void TypeWriter::startTyping(const QString& text, int speedMs)
{
    stop();  // Остановить предыдущую печать если была

    m_fullText = text;
    m_displayedText.clear();
    m_currentIndex = 0;
    m_speedMs = speedMs;

    emit speedChanged(m_speedMs);

    if (!m_fullText.isEmpty()) {
        m_timer.start(m_speedMs);
    }
}

void TypeWriter::skipToEnd()
{
    if (m_timer.isActive()) {
        m_timer.stop();
        m_displayedText = m_fullText;
        m_currentIndex = m_fullText.length();
        emit characterAdded(m_displayedText);
        emit typingFinished();
    }
}

void TypeWriter::stop()
{
    if (m_timer.isActive()) {
        m_timer.stop();
    }
    m_displayedText.clear();
    m_currentIndex = 0;
}

float TypeWriter::getProgress() const
{
    if (m_fullText.isEmpty()) {
        return 0.0f;
    }
    return static_cast<float>(m_currentIndex) / static_cast<float>(m_fullText.length());
}

void TypeWriter::onTimerTick()
{
    if (m_currentIndex < m_fullText.length()) {
        m_displayedText += m_fullText[m_currentIndex];
        m_currentIndex++;

        emit characterAdded(m_displayedText);

        if (m_currentIndex >= m_fullText.length()) {
            m_timer.stop();
            emit typingFinished();
        }
    }
}