#ifndef KEYBOARD_MOCK_HPP
#define KEYBOARD_MOCK_HPP

namespace BurgerEngine2D {
    using Key = BurgerEngine2D::KeyboardKey;

    void mockPressKey(Key key);
    void mockReleaseKey(Key key);
    void mockResetKeyboard();
    int mockGetQueryCount(Key key);

}

#endif