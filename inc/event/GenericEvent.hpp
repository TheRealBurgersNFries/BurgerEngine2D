
#ifndef GENERICEVENT_HPP
#define GENERICEVENT_HPP

#include "I_Event.hpp"

namespace BurgerEngine2D {
    template <typename T>
    class GenericEvent : public I_Event {
        private:
            EventType _type;
            T _data;

        public:
            GenericEvent(EventType type, T data)
                : _type(type), _data(data) {}

            EventType getType() const noexcept override {
                return _type;
            }

            const T& getData() const noexcept {
                return _data;
            }
    };
}

#endif // GENERICEVENT_HPP
