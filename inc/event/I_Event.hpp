#ifndef INTERFACE_EVENT_HPP
#define INTERFACE_EVENT_HPP

#include "EventType.hpp"

namespace BurgerEngine2D {

    class I_Event {
        public:
            virtual ~I_Event() = default;
            virtual EventType getType() const noexcept = 0;

    };

} // namespace BurgerEngine2D


#endif // INTERFACE_EVENT_HPPq