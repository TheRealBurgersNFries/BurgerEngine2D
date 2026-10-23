
#ifndef COLLISIONEVENT_HPP
#define COLLISIONEVENT_HPP

#include "EventType.hpp"
#include "GenericEvent.hpp"
#include "I_Collidable.hpp"

namespace BurgerEngine2D {
    struct CollisionEventData {
        I_Collidable* first;
        I_Collidable* second;
    };

    class CollisionEvent : public GenericEvent<CollisionEventData> {
        public:
            CollisionEvent(
                I_Collidable* first,
                I_Collidable* second
            );
    };

    inline CollisionEvent::CollisionEvent(
        I_Collidable* first,
        I_Collidable* second
    )
        : GenericEvent<CollisionEventData>(
            GetEventType<CollisionEvent>(),
            CollisionEventData{first, second}
        ) {}
}

#endif // COLLISIONEVENT_HPP
