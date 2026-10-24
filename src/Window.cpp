#include "Window.hpp"

#include "DrawStack2D.hpp"
#include "Vector2.hpp"

#include <thread>
#include <mutex>
#include <cmath>

namespace BurgerEngine2D {

    Window::Window(std::atomic<bool>* running, int width, int height,
        std::string title, int targetFrameRate, 
        BurgerEngine2D::DrawStack2D* stack, std::vector<int>* inputs, 
        std::atomic<bool>* inputsAvailable) {
            _windowWidth = width;
            _windowHeight = height;
            _title = title;
            _targetFramerate = targetFrameRate;
            _stack = stack;
            _running = running;

            _inputs = inputs;

            _inputsAvailable = inputsAvailable;

            auto renderThread = std::thread(&Window::renderTask, this);
            renderThread.detach();
        }
    
    void Window::renderTask() {
        initWindow(_windowWidth, _windowHeight, _title.c_str());
        setTargetFPS(_targetFramerate);
        int dt = std::ceil(1000000.0f / _targetFramerate);
        const auto interval = std::chrono::microseconds(dt);
        while (*_running) {
            auto startTime = std::chrono::steady_clock::now();
            if (!windowShouldClose()) {
                _stack->Draw(BurgerEngine2D::Vector2(_windowWidth/2, _windowHeight/2));
                //inputsAvailable = getInputs();
            } else {
                *_running = false;
            }
            auto endTime = std::chrono::steady_clock::now();
            auto elapsed = (endTime - startTime);
            #ifdef LOG_LEVEL_INFO
                int threadTime = (std::chrono::duration_cast<std::chrono::microseconds>(elapsed)).count();
                std::cout << _threadName + " Runtime:" + std::to_string(_threadTime) + "us" << std::endl;
            #endif
            if (elapsed < interval) {
                std::this_thread::sleep_for(interval - elapsed);
            }
        }
        closeWindow();
    }

}