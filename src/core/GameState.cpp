#include "GameState.h"
#include <QMetaType>

void registerGameStateType();

void registerGameStateType() {
    static bool registered = false;
    if (!registered) {
        []() {
            static int typeId = qRegisterMetaType<GameState>("GameState");
            Q_UNUSED(typeId);
        }();
        registered = true;
    }
}

namespace {
    struct GameStateTypeRegistration {
        GameStateTypeRegistration() {
            registerGameStateType();
        }
    } gameStateTypeRegistration;
}
