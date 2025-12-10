#include "TextGenerator.h"
#include "RandomGenerator.h"
#include <QRandomGenerator>

// Банки текстов для Древнего Замка
const QVector<QString> TextGenerator::CastleStarts = {
    "[Замок] Вы входите в величественный зал",
    "[Замок] Сводчатый потолок возвышается надо мной",
    "[Замок] На стенах видны отпечатки времени",
    "[Замок] Каменные колонны поддерживают крышу",
    "[Замок] Вы спускаетесь по узкому коридору замка",
    "[Замок] Древние гобелены украшают стены",
    "[Замок] Факелы освещают путь вперед"
};

const QVector<QString> TextGenerator::CastleMiddles = {
    ". Холодная каменная кладка",
    ". Эхо ваших шагов разносится по залам",
    ". Запах сырости и плесени заполняет воздух",
    ". Где-то вдалеке слышны странные звуки",
    ". На полу видны следы былого величия",
    ". Пыль веков оседает на ваших плечах",
    ". Морозный воздух пробирает до костей"
};

const QVector<QString> TextGenerator::CastleEnds = {
    ". Впереди видна деревянная дверь.",
    ". Путь ведет в неизвестность.",
    ". Вы готовы к новым испытаниям.",
    ". Ощущение опасности растет с каждым шагом.",
    ". Выбор предстоит перед вами.",
    ". Атмосфера становится все более зловещей.",
    ". Вы чувствуете, что вы не одни здесь."
};

// Банки текстов для Мистического Подземелья
const QVector<QString> TextGenerator::DungeonStarts = {
    "[Подземелье] Вы спускаетесь в темноту",
    "[Подземелье] Влага от стен капает на пол",
    "[Подземелье] Извилистые коридоры ведут вглубь",
    "[Подземелье] Подземелье предает вам мурашки",
    "[Подземелье] Вы слышите шорохи вокруг",
    "[Подземелье] Каменные коридоры тянутся вниз"
};

const QVector<QString> TextGenerator::DungeonMiddles = {
    ". Глухой звук эхо дважды отскочил от стен",
    ". Воздух становится все более тяжелым",
    ". На стенах видна непонятная разметка",
    ". Ощущение давления нарастает",
    ". Странные иероглифы украшают своды",
    ". Воздух насыщен влагой и холодом"
};

const QVector<QString> TextGenerator::DungeonEnds = {
    ". Дверь маячит в конце коридора.",
    ". Выход из подземелья не виден.",
    ". Вы должны выбрать правильный путь.",
    ". Интуиция подсказывает новое решение.",
    ". Загадка подземелья ждет ответа.",
    ". Тайна этого места еще не раскрыта."
};

// Банки текстов для Заброшенного Города
const QVector<QString> TextGenerator::CityStarts = {
    "[Город] Руины города встают перед вами",
    "[Город] Развалины зданий тянутся в небо",
    "[Город] Безмолвие города давит на психику",
    "[Город] Вы идете по разрушенным улицам",
    "[Город] Пустота города окружает вас",
    "[Город] Рухнувшие здания образуют лабиринт"
};

const QVector<QString> TextGenerator::CityMiddles = {
    ". Кирпичная пыль поднимается с каждым шагом",
    ". Окна пустых зданий смотрят пустыми глазами",
    ". Завалы обломков преграждают путь",
    ". Природа начинает отвоевывать город",
    ". Вьющиеся растения оплетают развалины",
    ". Скрип металла нарушает тишину"
};

const QVector<QString> TextGenerator::CityEnds = {
    ". Впереди видна более сохранившаяся постройка.",
    ". Путь ведет глубже в руины.",
    ". Вы чувствуете напряжение предчувствия.",
    ". Что-то манит вас дальше.",
    ". Прошлое этого места требует ответов.",
    ". Загадка города зовет вас вперед."
};

// Банки текстов для Теневого Леса
const QVector<QString> TextGenerator::ForestStarts = {
    "[Лес] Вы входите в густой лес",
    "[Лес] Ветви деревьев переплетаются над головой",
    "[Лес] Полумрак царит в чаще леса",
    "[Лес] Вы пробираетесь сквозь чащу",
    "[Лес] Лес кажется живым организмом",
    "[Лес] Деревья образуют естественный лабиринт"
};

const QVector<QString> TextGenerator::ForestMiddles = {
    ". Запах хвои и гнилого дерева наполняет ноздри",
    ". Листья шелестят под ногами",
    ". Где-то вдалеке слышны птичьи крики",
    ". Ощущение наблюдения не покидает вас",
    ". Лес дышит вместе с вами",
    ". Природа здесь кажется дикой и необузданной"
};

const QVector<QString> TextGenerator::ForestEnds = {
    ". Лесная тропа ведет вперед.",
    ". Свет впереди указывает на выход.",
    ". Вы готовы к встрече с неизвестным.",
    ". Лес медленно раскрывает свои тайны.",
    ". Погода в лесу может измениться в любой момент.",
    ". Следуйте инстинкту в этом дремучем лесу."
};

// Банки текстов для Кристального Дворца
const QVector<QString> TextGenerator::PalaceStarts = {
    "[Дворец] Вы входите в сияющий дворец",
    "[Дворец] Кристаллы светят из каждого уголка",
    "[Дворец] Свет преломляется через бриллианты",
    "[Дворец] Дворец сверкает как звезда",
    "[Дворец] Вы находитесь в хрустальном лабиринте",
    "[Дворец] Сияние кристаллов режет глаза"
};

const QVector<QString> TextGenerator::PalaceMiddles = {
    ". Звон кристаллов наполняет воздух",
    ". Отражения создают оптические иллюзии",
    ". Каждая грань дворца светит изнутри",
    ". Музыка кристаллов звучит на пороге слышимости",
    ". Цвета радуги танцуют на стенах",
    ". Дворец кажется живой сущностью"
};

const QVector<QString> TextGenerator::PalaceEnds = {
    ". Еще одна дверь из кристалла появляется перед вами.",
    ". Сверкание указывает на правильный путь.",
    ". Вы чувствуете магию этого места.",
    ". Дворец испытывает вашу достоинство.",
    ". Каждый шаг приносит новые откровения.",
    ". Секреты дворца ждут открытия."
};

QString TextGenerator::generateRoomDescription(
    int locationId,
    int roomNumber,
    const QString& locationName,
    const QString& locationTheme
)
{
    Q_UNUSED(roomNumber);
    Q_UNUSED(locationName);
    Q_UNUSED(locationTheme);

    // Выбираем соответствующие банки текстов
    const QVector<QString>* starts = &CastleStarts;
    const QVector<QString>* middles = &CastleMiddles;
    const QVector<QString>* ends = &CastleEnds;

    switch(locationId) {
        case 1:  // Ancient Castle
            starts = &CastleStarts;
            middles = &CastleMiddles;
            ends = &CastleEnds;
            break;
        case 2:  // Order Dungeon
            starts = &DungeonStarts;
            middles = &DungeonMiddles;
            ends = &DungeonEnds;
            break;
        case 3:  // Forgotten City
            starts = &CityStarts;
            middles = &CityMiddles;
            ends = &CityEnds;
            break;
        case 4:  // Shadow Forest
            starts = &ForestStarts;
            middles = &ForestMiddles;
            ends = &ForestEnds;
            break;
        case 5:  // Crystal Palace
            starts = &PalaceStarts;
            middles = &PalaceMiddles;
            ends = &PalaceEnds;
            break;
        default:
            starts = &CastleStarts;
            middles = &CastleMiddles;
            ends = &CastleEnds;
    }

    // Генерируем описание из трех частей
    QString description = selectFromVector(*starts);
    description += selectFromVector(*middles);
    description += selectFromVector(*ends);

    return description;
}

QString TextGenerator::generateMood(int locationId)
{
    QVector<QString> moods;

    switch(locationId) {
        case 1:
            moods = {"dark", "gloomy", "mysterious", "gothic"};
            break;
        case 2:
            moods = {"mystical", "cold", "oppressive", "sacred"};
            break;
        case 3:
            moods = {"desolate", "abandoned", "decaying", "forgotten"};
            break;
        case 4:
            moods = {"wild", "natural", "ancient", "dangerous"};
            break;
        case 5:
            moods = {"magical", "beautiful", "dazzling", "ethereal"};
            break;
        default:
            moods = {"unknown", "mysterious"};
    }

    return selectFromVector(moods);
}

QString TextGenerator::generateRandomEvent(int locationId)
{
    Q_UNUSED(locationId);
    
    QVector<QString> events = {
        "Слышен шорох позади вас.",
        "Ветер дует в спину.",
        "Температура резко упала.",
        "Вы заметили движение в тени.",
        "Запах изменился.",
        "Вы слышите отдаленный звук.",
        "Что-то упало на пол позади вас."
    };

    return selectFromVector(events);
}

QString TextGenerator::selectFromVector(const QVector<QString>& vector)
{
    if (vector.isEmpty()) {
        return "";
    }

    int randomIndex = RandomGenerator::random(0, vector.size() - 1);
    return vector[randomIndex];
}