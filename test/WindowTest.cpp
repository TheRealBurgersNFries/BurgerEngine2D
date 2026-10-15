#include "Window.hpp"

#include "I_Drawable.hpp"
#include "DrawStack2D.hpp"

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
        stack = BurgerEngine2D::DrawStack2D();
        running = true;
        inputsAvail = false;
        inputs = std::vector<int>();

    }

    void teardown() {
        running = false;
    }

    void init() {
        std::atomic<bool>* runPtr = running ? std::addressof(running.value()) : nullptr;
        std::atomic<bool>* availPtr = inputsAvail ? std::addressof(inputsAvail.value()) : nullptr; 
        BurgerEngine2D::DrawStack2D* stackPtr = stack ? std::addressof(stack.value()) : nullptr;
        std::vector<int>* inputsPtr = inputs ? std::addressof(inputs.value()) : nullptr;

        testWindow = BurgerEngine2D::Window(runPtr, width, height,title,60, stackPtr, inputsPtr, availPtr);
    }
};

TEST(WindowTestGroup, Initialize) {
    mock().ignoreOtherCalls();

    init();
    running = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}
