
#include "KeyboardController.hpp"

#include "Dispatcher.hpp"
#include "KeyPressedEvent.hpp"

#include <algorithm>
#include <utility>

namespace BurgerEngine2D {
    using Key = BurgerEngine2D::KeyboardKey;
    using SubscriptionId = std::size_t;

    KeyboardController::KeyboardController(KeyboardWrapper& keyboard)
        : _keyboard(keyboard) {}

    SubscriptionId KeyboardController::subscribe(Key key, KeyCallback callback) {
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
            struct ActiveBinding {
                Key key;
                std::vector<KeyCallback> callbacks;
            };

        std::vector<ActiveBinding> activeBindings;
        activeBindings.reserve(_keyRegistry.size());

        for (const auto& [key, listeners] : _keyRegistry) {
            if (listeners.empty() || !_keyboard.isKeyPressed(key))
                continue;

            ActiveBinding binding;
            binding.key = key;
            binding.callbacks.reserve(listeners.size());

            for (const auto& listener : listeners) {
                binding.callbacks.push_back(listener.callback);
            }

            activeBindings.push_back(std::move(binding));
        }

        for (const auto& binding : activeBindings) {
            Dispatcher::queueEvent(std::make_shared<KeyPressedEvent>(
                    static_cast<KeyboardKey>(binding.key)
                )
            );

            for (const auto& callback : binding.callbacks) {
                if (callback)
                    callback(binding.key);
            }
        }
    }
}