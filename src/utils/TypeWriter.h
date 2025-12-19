#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

class TypeWriter : public QObject {
    Q_OBJECT

public:
    explicit TypeWriter(QObject* parent = nullptr);
    ~TypeWriter();

    void startTyping(const QString& text, int speedMs = 50);
    void skipToEnd();
    void stop();
    const QString& getCurrentText() const { return m_displayedText; }
    const QString& getFullText() const { return m_fullText; }
    bool isTyping() const { return m_timer.isActive(); }
    float getProgress() const;

    signals:
        void characterAdded(const QString& text);

    void typingFinished();

    void speedChanged(int ms);

private slots:
    void onTimerTick();

private:
    QTimer m_timer;
    QString m_fullText;
    QString m_displayedText;
    int m_currentIndex = 0;
    int m_speedMs = 50;
};

