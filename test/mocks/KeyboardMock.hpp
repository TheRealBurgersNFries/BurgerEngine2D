#ifndef KEYBOARD_MOCK_HPP
#define KEYBOARD_MOCK_HPP

namespace BurgerEngine2D {

    void mockPressKey(int key);
    void mockReleaseKey(int key);
    void mockResetKeyboard();
    int mockGetQueryCount(int key);

}

#endif