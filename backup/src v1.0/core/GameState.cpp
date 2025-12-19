#include "GameState.h"
#include <QMetaType>

// Register GameState with Qt's meta-object system
void registerGameStateType() {
    static bool registered = false;
    if (!registered) {
        qRegisterMetaType<GameState>("GameState");
        registered = true;
    }
}

// Force registration at library load time
namespace {
    struct GameStateTypeRegistration {
        GameStateTypeRegistration() {
            registerGameStateType();
        }
    } gameStateTypeRegistration;
}
