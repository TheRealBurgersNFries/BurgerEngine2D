#ifndef UPDATECOMPLETEEVENT_HPP
#define UPDATECOMPLETEEVENT_HPP

#include "EventType.hpp"
#include "GenericEvent.hpp"

#include <chrono>
namespace BurgerEngine2D{
    struct UpdateCompleteEventData {
        std::chrono::microseconds _updateTime;
    };

    class UpdateCompleteEvent : public GenericEvent<UpdateCompleteEventData> {
        public:
            UpdateCompleteEvent( std::chrono::microseconds
     updateTime);
    };

    inline UpdateCompleteEvent::UpdateCompleteEvent(std::chrono::microseconds updateTime) :
        GenericEvent<UpdateCompleteEventData>(GetEventType<UpdateCompleteEvent>(), 
        UpdateCompleteEventData{updateTime}) {

        }

}

#endif 