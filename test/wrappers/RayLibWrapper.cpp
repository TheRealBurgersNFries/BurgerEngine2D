#include "RayLibWrapper.hpp"
#include "CppUTestExt/MockSupport.h"

namespace BurgerEngine2D {
    void beginDrawing() {
        mock().actualCall("beginDrawing");
    }

    void endDrawing() {
        mock().actualCall("endDrawing");
    }
}