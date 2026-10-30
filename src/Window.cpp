#include "Window.hpp"

#include "DrawStack2D.hpp"
#include "Vector2.hpp"
#include "UpdateCompleteEvent.hpp"
#include "RenderCompleteEvent.hpp"

#include <memory>
#include <thread>
#include <mutex>
#include <cmath>
#include <chrono>

namespace BurgerEngine2D {

    Window::Window(std::atomic<bool>* running, int width, int height,
        std::string title, int targetFrameRate, 
        BurgerEngine2D::DrawStack2D* stack, std::vector<int>* inputs, 
        std::atomic<bool>* inputsAvailable) :
            _running(running), _windowWidth(width),_windowHeight(height),
            _title(title), _targetFramerate(targetFrameRate), _stack(stack), 
            _inputs(inputs), _inputsAvailable(inputsAvailable), 
            _physicsComplete(false) {

            _renderThread = std::jthread(&Window::renderTask, this);
            Dispatcher::addSubscriber(GetEventType<UpdateCompleteEvent>(), this);
        }

    Window::~Window() {
        if (_running != nullptr) {
            _running->store(false);
        }

        // Wake the render thread if it is blocked waiting for a physics event.
        _physicsCondition.notify_all();

        if (_renderThread.joinable()) {
            _renderThread.join();
        }
    }
    
    void Window::onEvent(std::shared_ptr<const I_Event> event) {
        if (!event ||
            event->getType() != GetEventType<UpdateCompleteEvent>()) {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(_physicsMutex);
            _physicsComplete = true;
        }

        _physicsCondition.notify_one();
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
                // _inputsAvailable = getInputs();
            } else {
                *_running = false;
            }
            auto endTime = std::chrono::steady_clock::now();
            auto elapsed = (endTime - startTime);
            Dispatcher::queueEvent(std::make_shared<RenderCompleteEvent>(
                    std::chrono::duration_cast<std::chrono::microseconds>(elapsed)));
            #ifdef LOG_LEVEL_INFO
                int threadTime = (std::chrono::duration_cast<std::chrono::microseconds>(elapsed)).count();
                std::cout << _threadName + " Runtime:" + std::to_string(_threadTime) + "us" << std::endl;
            #endif
            if (elapsed < interval) {
                std::this_thread::sleep_for(interval - elapsed);
            }
            std::unique_lock<std::mutex> lock(_physicsMutex);
            _physicsCondition.wait(lock, [this] {
                return _physicsComplete ||
                    (_running != nullptr && !_running->load());
            });

            if (_running == nullptr || !_running->load()) {
                break;
            }

            // Consume the completion signal before beginning the next frame.
            _physicsComplete = false;
            
        }
        closeWindow();
    }

}
