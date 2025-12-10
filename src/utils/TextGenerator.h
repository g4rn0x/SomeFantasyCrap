#pragma once

#include <QString>
#include <QVector>

/**
 * @brief TextGenerator - Генератор уникальных описаний комнат для каждой локации
 * Создает атмосферный текст для каждой комнаты и каждой локации
 */
class TextGenerator {
public:
    TextGenerator() = delete;  // Статический класс

    /**
     * @brief Сгенерировать описание комнаты
     * @param locationId - ID локации (1-15)
     * @param roomNumber - номер комнаты (1-10)
     * @param locationName - название локации
     * @param locationTheme - тема локации
     * @return Текст описания комнаты
     */
    static QString generateRoomDescription(
        int locationId,
        int roomNumber,
        const QString& locationName,
        const QString& locationTheme
    );

    /**
     * @brief Получить атмосферу локации
     */
    static QString generateMood(int locationId);

    /**
     * @brief Генерировать случайное событие (звук, запах и т.д.)
     */
    static QString generateRandomEvent(int locationId);

private:
    // Банки текстов для древнего замка
    static const QVector<QString> CastleStarts;
    static const QVector<QString> CastleMiddles;
    static const QVector<QString> CastleEnds;

    // Банки текстов для подземелья
    static const QVector<QString> DungeonStarts;
    static const QVector<QString> DungeonMiddles;
    static const QVector<QString> DungeonEnds;

    // Банки текстов для заброшенного города
    static const QVector<QString> CityStarts;
    static const QVector<QString> CityMiddles;
    static const QVector<QString> CityEnds;

    // Банки текстов для теневого леса
    static const QVector<QString> ForestStarts;
    static const QVector<QString> ForestMiddles;
    static const QVector<QString> ForestEnds;

    // Банки текстов для кристального дворца
    static const QVector<QString> PalaceStarts;
    static const QVector<QString> PalaceMiddles;
    static const QVector<QString> PalaceEnds;

    static QString selectFromVector(const QVector<QString>& vector);
};