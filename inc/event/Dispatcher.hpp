
#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "EventType.hpp"
#include "I_Event.hpp"
#include "I_Subscriber.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <queue>
#include <vector>

namespace BurgerEngine2D {

    class Dispatcher {
        public:
            static void queueEvent(std::shared_ptr<const I_Event> event);
            static void dispatch();

            static void addSubscriber(EventType type, I_Subscriber* subscriber);
            static void removeSubscriber(I_Subscriber* subscriber);
            static void removeSubscription(EventType type, I_Subscriber* subscriber);

            static void clearEvent(EventType type);
            static void clearAllSubscribers();

        private:
            Dispatcher() = default;
            ~Dispatcher() = default;

            static void dispatchEvent(std::shared_ptr<const I_Event> event);

            static std::map<EventType, std::vector<I_Subscriber*>> _subscribers;
            static std::queue<std::shared_ptr<const I_Event>> _eventQueue;
    };

}

#endif // DISPATCHER_HPP
