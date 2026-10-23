#ifndef INTERFACE_PUBLISHER_HPP
#define INTERFACE_PUBLISHER_HPP

#include "I_Subscriber.hpp"
#include "EventType.hpp"

#include <memory>

namespace BurgerEngine2D {
    class I_Publisher {
        public:
            
            virtual ~I_Publisher() = default;
            virtual void Publish(const std::unique_ptr<BurgerEngine2D::EventType> & event) = 0;
            

    };
}

#endif