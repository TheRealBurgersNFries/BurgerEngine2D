#include "wrappers/KeyboardWrapper.hpp"
#include "raylib.h"

namespace BurgerEngine2D {

    bool KeyboardWrapper::isKeyPressed(int key) {
        return IsKeyDown(key);
    }

}