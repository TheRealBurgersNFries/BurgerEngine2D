#ifndef KEYBOARD_WRAPPER_HPP
#define KEYBOARD_WRAPPER_HPP

namespace BurgerEngine2D {
    enum class KeyboardKey : int;
    using Key = BurgerEngine2D::KeyboardKey;

    class KeyboardWrapper {
    public:
        static bool isKeyPressed(Key key);
    };

}

#endif