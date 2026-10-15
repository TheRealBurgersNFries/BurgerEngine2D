#include "RayLibWrapper.hpp"
#include "raylib.h"

namespace BurgerEngine2D {
    void beginDrawing() {
        BeginDrawing();
    }

    void endDrawing() {
        EndDrawing();
    }

    void initWindow(int width, int height, const char* title) {
        InitWindow(width, height, title);
    }

    void closeWindow() {
        CloseWindow();
    }

    void setTargetFPS(int target) {
        SetTargetFPS(target);
    }

    bool windowShouldCLose() {
        WindowShouldClose();
    }
}