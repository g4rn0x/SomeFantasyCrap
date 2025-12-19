#pragma once

#include <QString>
#include <QVector>

class TextGenerator {
public:
    TextGenerator() = delete;

    static QString generateRoomDescription(
        int locationId,
        int roomNumber,
        const QString& locationName,
        const QString& locationTheme
    );

    static QString generateMood(int locationId);

    static QString generateRandomEvent(int locationId);

private:
    static const QVector<QString> CastleStarts;
    static const QVector<QString> CastleMiddles;
    static const QVector<QString> CastleEnds;

    static const QVector<QString> DungeonStarts;
    static const QVector<QString> DungeonMiddles;
    static const QVector<QString> DungeonEnds;

    static const QVector<QString> CityStarts;
    static const QVector<QString> CityMiddles;
    static const QVector<QString> CityEnds;

    static const QVector<QString> ForestStarts;
    static const QVector<QString> ForestMiddles;
    static const QVector<QString> ForestEnds;

    static const QVector<QString> PalaceStarts;
    static const QVector<QString> PalaceMiddles;
    static const QVector<QString> PalaceEnds;

    static QString selectFromVector(const QVector<QString>& vector);
};

