#include "RayLibWrapper.hpp"
#include "CppUTestExt/MockSupport.h"

namespace BurgerEngine2D {
    void beginDrawing() {
        mock().actualCall("beginDrawing");
    }

    void endDrawing() {
        mock().actualCall("endDrawing");
    }

    void initWindow([[maybe_unused]] int width, [[maybe_unused]] int height, [[maybe_unused]] const char* title) {
        
        mock().actualCall("initWIndow");
    }

    void closeWindow() {
        mock().actualCall("closeWindow");
    }

    void setTargetFPS( [[maybe_unused]] int target) {
        mock().actualCall("setTargetFPS");
    }

    bool windowShouldClose() {
        mock().actualCall("windowShouldClose");
        return false;
    }
}