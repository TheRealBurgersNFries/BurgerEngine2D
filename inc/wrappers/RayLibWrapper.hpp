#ifndef RAYLIB_WRAPPER_HPP
#define RAYLIB_WRAPPER_HPP

namespace BurgerEngine2D {
    void beginDrawing();
    void endDrawing();
    void initWindow(int width, int height, const char* title);
    void closeWindow();
    void setTargetFPS(int target);
    bool windowShouldClose();
}

#endif // RAYLIB_WRAPPER_HPP