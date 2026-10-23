
#ifndef KEYDOWNEVENT_HPP
#define KEYDOWNEVENT_HPP

#include "EventType.hpp"
#include "GenericEvent.hpp"
#include "KeyboardKey.hpp"

namespace BurgerEngine2D {
    struct KeyDownEventData {
        KeyboardKey key;
    };

    class KeyDownEvent : public GenericEvent<KeyDownEventData> {
        public:
            explicit KeyDownEvent(KeyboardKey key);
    };

    inline KeyDownEvent::KeyDownEvent(KeyboardKey key)
        : GenericEvent<KeyDownEventData>(
            GetEventType<KeyDownEvent>(),
            KeyDownEventData{key}
        ) {}
}

#endif // KEYDOWNEVENT_HPP
