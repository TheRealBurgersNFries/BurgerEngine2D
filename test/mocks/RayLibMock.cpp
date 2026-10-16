#include "RayLibWrapper.hpp"
#include "RayLibMock.hpp"
#include "CppUTestExt/MockSupport.h"

#include <atomic>

namespace BurgerEngine2D {

    inline std::atomic<int> beginDrawingCalls{0};
    inline std::atomic<int> endDrawingCalls{0};
    inline std::atomic<int> initWindowCalls{0};
    inline std::atomic<int> closeWindowCalls{0};
    inline std::atomic<int> setTargetFPSCalls{0};
    inline std::atomic<int> windowShouldCloseCalls{0};

    void beginDrawing() {
        mock().actualCall("beginDrawing");
        beginDrawingCalls.fetch_add(1);
    }

    void endDrawing() {
        mock().actualCall("endDrawing");
        endDrawingCalls.fetch_add(1);
    }

    void initWindow([[maybe_unused]] int width, [[maybe_unused]] int height, [[maybe_unused]] const char* title) {
        mock().actualCall("initWIndow");
        initWindowCalls.fetch_add(1);
    }

    void closeWindow() {
        mock().actualCall("closeWindow");
        closeWindowCalls.fetch_add(1);
    }

    void setTargetFPS( [[maybe_unused]] int target) {
        mock().actualCall("setTargetFPS");
        setTargetFPSCalls.fetch_add(1);
    }

    bool windowShouldClose() {
        mock().actualCall("windowShouldClose");
        windowShouldCloseCalls.fetch_add(1);
        return false;
    }

    void resetMockCalls() {
        beginDrawingCalls = 0;
        endDrawingCalls = 0;
        initWindowCalls = 0;
        closeWindowCalls = 0;
        setTargetFPSCalls = 0;
        windowShouldCloseCalls = 0;
    }

    int getBeginDrawingCalls() {
        return beginDrawingCalls;
    }

    int getEndDrawingCalls() {
        return endDrawingCalls;
    }

    int getInitWindowCalls() {
        return initWindowCalls;
    }

    int getCloseWindowCalls() {
        return closeWindowCalls;
    }

    int getSetTargetCalls() {
        return setTargetFPSCalls;
    }

    int getWindowCloseCalls() {
        return windowShouldCloseCalls;
    }
    
}