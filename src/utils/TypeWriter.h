#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

/**
 * @brief TypeWriter - Эффект печатания текста как в визуальных новеллах
 * Выводит текст посимвольно с настраиваемой скоростью
 */
class TypeWriter : public QObject {
    Q_OBJECT

public:
    explicit TypeWriter(QObject* parent = nullptr);
    ~TypeWriter();

    /**
     * @brief Начать печать текста
     * @param text - полный текст для печати
     * @param speedMs - задержка между символами (мс)
     */
    void startTyping(const QString& text, int speedMs = 50);

    /**
     * @brief Пропустить печать и показать полный текст
     */
    void skipToEnd();

    /**
     * @brief Остановить печать
     */
    void stop();

    /**
     * @brief Получить текущий отпечатанный текст
     */
    const QString& getCurrentText() const { return m_displayedText; }

    /**
     * @brief Получить полный текст
     */
    const QString& getFullText() const { return m_fullText; }

    /**
     * @brief Проверить, идет ли печать
     */
    bool isTyping() const { return m_timer.isActive(); }

    /**
     * @brief Получить прогресс печати (0.0 - 1.0)
     */
    float getProgress() const;

    signals:
        /**
         * @brief Сигнал при добавлении символа
         */
        void characterAdded(const QString& text);

    /**
     * @brief Сигнал при завершении печати
     */
    void typingFinished();

    /**
     * @brief Сигнал при изменении скорости
     */
    void speedChanged(int ms);

private slots:
    /**
     * @brief Обработчик тика таймера
     */
    void onTimerTick();

private:
    QTimer m_timer;
    QString m_fullText;
    QString m_displayedText;
    int m_currentIndex = 0;
    int m_speedMs = 50;
};