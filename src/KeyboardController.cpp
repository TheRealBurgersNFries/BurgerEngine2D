#include "KeyboardController.hpp"

#include <algorithm>
#include <utility>

namespace BurgerEngine2D {

    KeyboardController::KeyboardController(KeyboardWrapper& keyboard)
        : _keyboard(keyboard) {}

    SubscriptionId KeyboardController::subscribe(
        int key,
        KeyCallback callback) {
        const SubscriptionId id = _nextId++;

        _keyRegistry[key].push_back({id, std::move(callback)});

        return id;
    }

    void KeyboardController::unsubscribe(SubscriptionId id) {
        for (auto key = _keyRegistry.begin();
             key != _keyRegistry.end();) {

            auto& listeners = key->second;

            std::erase_if(listeners, [id](const Listener& listener) {
                return listener.id == id;
            });

            if (listeners.empty()) {
                key = _keyRegistry.erase(key);
            } else {
                ++key;
            }
        }
    }

    void KeyboardController::update() {
        for (const auto& [key, listeners] : _keyRegistry) {
            if (!_keyboard.isKeyPressed(key)) {
                continue;
            }

            std::vector<KeyCallback> callbacks;
            callbacks.reserve(listeners.size());

            for (const auto& listener : listeners) {
                callbacks.push_back(listener.callback);
            }

            for (const auto& callback : callbacks) {
                callback(key);
            }
        }
    }
}