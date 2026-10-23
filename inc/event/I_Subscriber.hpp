#ifndef INTERFACE_SUBSCRIBER_HPP
#define INTERFACE_SUBSCRIBER_HPP

#include <memory>

#include "I_Event.hpp"

namespace BurgerEngine2D {
    class I_Subscriber {
        public:
            virtual ~I_Subscriber() = default;
            virtual void onEvent(std::shared_ptr<const I_Event> event) = 0;
    };
}

#endif // INTERFACE_SUBSCRIBER_HPP