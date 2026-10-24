
#ifndef KEYPRESSEDEVENT_HPP
#define KEYPRESSEDEVENT_HPP

#include "EventType.hpp"
#include "GenericEvent.hpp"
#include "KeyboardKey.hpp"

namespace BurgerEngine2D {
    struct KeyPressedEventData {
        KeyboardKey key;
    };

    class KeyPressedEvent : public GenericEvent<KeyPressedEventData> {
        public:
            explicit KeyPressedEvent(KeyboardKey key);
    };

    inline KeyPressedEvent::KeyPressedEvent(KeyboardKey key)
        : GenericEvent<KeyPressedEventData>(
            GetEventType<KeyPressedEvent>(),
            KeyPressedEventData{key}
        ) {}
}

#endif // KEYPRESSEDEVENT_HPP
