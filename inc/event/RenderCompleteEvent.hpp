#ifndef RENDERCOMPLETEEVENT_HPP
#define RENDERCOMPLETEEVENT_HPP

#include "EventType.hpp"
#include "GenericEvent.hpp"

#include <chrono>

namespace BurgerEngine2D {
    struct RenderCompleteEventData {
        std::chrono::microseconds _renderTime;
    };

    class RenderCompleteEvent : public GenericEvent<RenderCompleteEventData> {
        public:
            RenderCompleteEvent( std::chrono::microseconds renderTime);
    };

    inline RenderCompleteEvent::RenderCompleteEvent(std::chrono::microseconds renderTime) :
        GenericEvent<RenderCompleteEventData>(GetEventType<RenderCompleteEvent>(), 
            RenderCompleteEventData{renderTime}) {
    }
}



#endif // RENDERCOMPLETEEVENET_HPP