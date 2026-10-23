
#include "Dispatcher.hpp"

#include <algorithm>
#include <utility>

namespace BurgerEngine2D {

    std::map<EventType, std::vector<I_Subscriber*>>
        Dispatcher::_subscribers;

    std::queue<std::shared_ptr<const I_Event>>
        Dispatcher::_eventQueue;

    void Dispatcher::queueEvent(std::shared_ptr<const I_Event> event) {
        if (event) {
            _eventQueue.push(std::move(event));
        }
    }

    void Dispatcher::dispatch() {
        const std::size_t eventsToDispatch = _eventQueue.size();

        for (std::size_t i = 0; i < eventsToDispatch; ++i) {
            if (_eventQueue.empty()) {
                break;
            }

            auto event = std::move(_eventQueue.front());
            _eventQueue.pop();

            dispatchEvent(std::move(event));
        }
    }

    void Dispatcher::addSubscriber(EventType type, I_Subscriber* subscriber) {
        if (subscriber == nullptr) {
            return;
        }

        auto& subscribers = _subscribers[type];

        if (std::find(subscribers.begin(), subscribers.end(), subscriber)
            == subscribers.end()) {
            subscribers.push_back(subscriber);
        }
    }

    void Dispatcher::removeSubscriber(I_Subscriber* subscriber) {
        for (auto& [type, subscribers] : _subscribers) {
            subscribers.erase(
                std::remove(subscribers.begin(), subscribers.end(), subscriber),
                subscribers.end()
            );
        }
    }

    void Dispatcher::removeSubscription(
        EventType type,
        I_Subscriber* subscriber
    ) {
        auto it = _subscribers.find(type);

        if (it == _subscribers.end()) {
            return;
        }

        auto& subscribers = it->second;

        subscribers.erase(
            std::remove(subscribers.begin(), subscribers.end(), subscriber),
            subscribers.end()
        );

        if (subscribers.empty()) {
            _subscribers.erase(it);
        }
    }

    void Dispatcher::clearEvent(EventType type) {
        _subscribers.erase(type);
    }

    void Dispatcher::clearAllSubscribers() {
        _subscribers.clear();
    }

    void Dispatcher::dispatchEvent(std::shared_ptr<const I_Event> event) {
        if (!event) {
            return;
        }

        const EventType type = event->getType();
        auto it = _subscribers.find(type);

        if (it == _subscribers.end()) {
            return;
        }

        // Copy the list so subscription changes cannot invalidate iteration.
        const auto subscribers = it->second;

        for (I_Subscriber* subscriber : subscribers) {
            if (subscriber == nullptr) {
                continue;
            }

            // Skip subscribers removed during an earlier callback.
            auto current = _subscribers.find(type);

            if (current == _subscribers.end()) {
                break;
            }

            const auto& currentSubscribers = current->second;

            if (std::find(
                    currentSubscribers.begin(),
                    currentSubscribers.end(),
                    subscriber
                ) == currentSubscribers.end()) {
                continue;
            }

            subscriber->onEvent(event);
        }
    }

}
