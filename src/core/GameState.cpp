#include "GameState.h"
#include <QMetaType>

// Forward declare the type registration function
void registerGameStateType();

// Register GameState with Qt's meta-object system
void registerGameStateType() {
    static bool registered = false;
    if (!registered) {
        // Use a lambda to avoid direct template instantiation issues
        []() {
            static int typeId = qRegisterMetaType<GameState>("GameState");
            Q_UNUSED(typeId);
        }();
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
