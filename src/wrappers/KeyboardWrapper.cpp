#include "wrappers/KeyboardWrapper.hpp"
#include "raylib.h"

#include "KeyboardKey.hpp"

namespace BurgerEngine2D {
    using Key = BurgerEngine2D::KeyboardKey;

    bool KeyboardWrapper::isKeyPressed(Key key) {
        return IsKeyDown(static_cast<int>(key));
    }

}