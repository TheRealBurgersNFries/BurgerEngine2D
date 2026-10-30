#include "Engine.hpp"

#include <chrono>
#include <stdexcept>
#include <thread>
#include <utility>

#include "Dispatcher.hpp"
#include "Window.hpp"

namespace BurgerEngine2D {

    Engine::Engine(const EngineConfig& config)
        : _config(config),
          _running(false),
          _inputsAvailable(false),
          _inputs(),
          _drawStack(),
          _dispatcher(),
          _physicsSystem(nullptr),
          _window() {
        if (_config.windowWidth <= 0 || _config.windowHeight <= 0) {
            throw std::invalid_argument("Engine window dimensions must be positive.");
        }

        if (_config.targetFrameRate <= 0) {
            throw std::invalid_argument("Engine target frame rate must be positive.");
        }
    }

    Engine::~Engine() {
        stop();
        _window.reset();
    }

    void Engine::run() {
        bool expected = false;
        if (!_running.compare_exchange_strong(expected, true)) {
            return;
        }

        try {
            _window = std::make_unique<Window>(
                &_running,
                _config.windowWidth,
                _config.windowHeight,
                _config.windowTitle,
                _config.targetFrameRate,
                &_drawStack,
                &_inputs,
                &_inputsAvailable);
            _physicsSystem.start();
        } catch (...) {
            _running.store(false);
            _physicsSystem.stop();
            _window.reset();
            throw;
        }

        // Keep event callbacks on the main thread while worker threads queue
        // events for the next iteration.
        while (_running.load()) {
            Dispatcher::dispatch();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        _physicsSystem.stop();
        Dispatcher::dispatch();
        _window.reset();
    }

    void Engine::stop() {
        _running.store(false);
        _physicsSystem.stop();
    }

    bool Engine::isRunning() const noexcept {
        return _running.load();
    }

    bool Engine::addDrawable(I_Drawable& drawable) {
        return _drawStack.addToStack(&drawable);
    }

    void Engine::removeDrawable(I_Drawable& drawable) {
        _drawStack.removeFromStack(&drawable);
    }

    void Engine::addCollidable(I_Collidable& collidable) {
        _physicsSystem.addCollidable(collidable);
    }

    void Engine::removeCollidable(I_Collidable& collidable) {
        _physicsSystem.removeCollidable(collidable);
    }

    void Engine::queueEvent(std::shared_ptr<const I_Event> event) {
        Dispatcher::queueEvent(std::move(event));
    }

    void Engine::addEventSubscriber(
        EventType type,
        I_Subscriber& subscriber
    ) {
        Dispatcher::addSubscriber(type, &subscriber);
    }

    void Engine::removeEventSubscriber(
        EventType type,
        I_Subscriber& subscriber
    ) {
        Dispatcher::removeSubscription(type, &subscriber);
    }

    void Engine::removeEventSubscriber(I_Subscriber& subscriber) {
        Dispatcher::removeSubscriber(&subscriber);
    }

    DrawStack2D& Engine::getDrawStack() noexcept {
        return _drawStack;
    }

    PhysicsSystem2D& Engine::getPhysicsSystem() noexcept {
        return _physicsSystem;
    }

    Dispatcher& Engine::getDispatcher() noexcept {
        return _dispatcher;
    }

}
