
#ifndef EVENTTYPE_H
#define EVENTTYPE_H

#include <cstdint>
#include <typeindex>
#include <typeinfo>
#include <type_traits>

namespace BurgerEngine2D
{
    using EventType = std::uint64_t;

    namespace detail
    {
        EventType RegisterEventType(std::type_index type);
    }

    template <typename T>
    EventType GetEventType()
    {
        static_assert(
            std::is_class_v<T>,
            "Event types must be class types."
        );

        return detail::RegisterEventType(
            std::type_index(typeid(T))
        );
    }
}

#endif // BURGERENGINE2D_EVENTTYPE_H
