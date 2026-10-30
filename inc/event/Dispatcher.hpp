
#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "EventType.hpp"
#include "I_Event.hpp"
#include "I_Subscriber.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

namespace BurgerEngine2D {

    class Dispatcher {
        public:
            Dispatcher() = default;
            ~Dispatcher() = default;

            Dispatcher(const Dispatcher&) = delete;
            Dispatcher& operator=(const Dispatcher&) = delete;

            static void queueEvent(std::shared_ptr<const I_Event> event);
            static void dispatch();

            static void addSubscriber(EventType type, I_Subscriber* subscriber);
            static void removeSubscriber(I_Subscriber* subscriber);
            static void removeSubscription(EventType type, I_Subscriber* subscriber);

            static void clearEvent(EventType type);
            static void clearAllSubscribers();

        private:
            static void dispatchEvent(std::shared_ptr<const I_Event> event);

            static std::map<EventType, std::vector<I_Subscriber*>> _subscribers;
            static std::vector<std::shared_ptr<const I_Event>> _eventQueue;
            static std::mutex _eventQueueMutex;
    };

}

#endif // DISPATCHER_HPP
