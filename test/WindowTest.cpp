#include "Window.hpp"

#include "I_Drawable.hpp"
#include "DrawStack2D.hpp"

#include "RayLibWrapper.hpp"
#include "RayLibMock.hpp"

#include <optional>

#include <atomic>
#include <string>
#include <thread>


#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

#include "CppUTestExt/MockSupport.h"

TEST_GROUP(WindowTestGroup) {

    std::optional<BurgerEngine2D::Window> testWindow = std::nullopt;

    std::optional<BurgerEngine2D::DrawStack2D> stack = std::nullopt;

    std::optional<std::atomic<bool>> running = std::nullopt;
    std::optional<std::atomic<bool>> inputsAvail = std::nullopt;

    std::optional<std::vector<int>> inputs = std::nullopt;

    int width = 800;
    int height = 600; 
    std::string title = "test";
    
    
    void setup() {
        stack.emplace();
        running = true;
        inputsAvail = false;
        inputs = std::vector<int>();
    }

    void teardown() {
        running = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        BurgerEngine2D::resetMockCalls();
    }

    void init() {
        std::atomic<bool>* runPtr = running ? std::addressof(running.value()) : nullptr;
        std::atomic<bool>* availPtr = inputsAvail ? std::addressof(inputsAvail.value()) : nullptr; 
        BurgerEngine2D::DrawStack2D* stackPtr = stack ? std::addressof(stack.value()) : nullptr;
        std::vector<int>* inputsPtr = inputs ? std::addressof(inputs.value()) : nullptr;

        testWindow.emplace(runPtr, width, height,title,60, stackPtr, inputsPtr, availPtr);
    }
};

TEST(WindowTestGroup, initializeAndExit) {
    mock().ignoreOtherCalls();
    int test = BurgerEngine2D::getCloseWindowCalls();
    test++;
    init();
    running = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    CHECK_EQUAL(1, BurgerEngine2D::getInitWindowCalls());
    CHECK_EQUAL(1, BurgerEngine2D::getCloseWindowCalls());
    

}

TEST(WindowTestGroup, defaultRenderTiming) {
    mock().ignoreOtherCalls();
    
    init();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    running = false;
    int upperBound = 121;
    int lowerBound = 119;

    int beginCalls = BurgerEngine2D::getBeginDrawingCalls();
    int endCalls = BurgerEngine2D::getEndDrawingCalls();
    CHECK_COMPARE(lowerBound, <=, beginCalls);
    CHECK_COMPARE(upperBound, >=, beginCalls);
    CHECK_COMPARE(lowerBound, <=, endCalls);
    CHECK_COMPARE(upperBound, >=, endCalls);
}

TEST(WindowTestGroup, longRenderTiming) {
    BurgerEngine2D::setDrawDelay(10);

    mock().ignoreOtherCalls();
    
    init();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    running = false;
    int upperBound = 121;
    int lowerBound = 119;

    int beginCalls = BurgerEngine2D::getBeginDrawingCalls();
    int endCalls = BurgerEngine2D::getEndDrawingCalls();
    CHECK_COMPARE(lowerBound, <=, beginCalls);
    CHECK_COMPARE(upperBound, >=, beginCalls);
    CHECK_COMPARE(lowerBound, <=, endCalls);
    CHECK_COMPARE(upperBound, >=, endCalls);

    BurgerEngine2D::resetDrawDelay();
}
