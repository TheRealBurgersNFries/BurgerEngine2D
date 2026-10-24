
#ifndef EVENTTYPE_HPP
#define EVENTTYPE_HPP

#include <atomic>
#include <cstdint>
#include <type_traits>
#include <stdexcept>

namespace BurgerEngine2D {

    using EventType = std::uint64_t;

    namespace detail {

        inline EventType NextEventType() {
            static std::atomic<EventType> nextType{0};

            const EventType type = nextType.fetch_add(
                1,
                std::memory_order_relaxed
            );

            if (type == UINT64_MAX) {
                throw std::overflow_error(
                    "Event type identifiers exhausted."
                );
            }

            return type;
        }

        template <typename T>
        EventType RegisterEventType() {
            static const EventType type = NextEventType();
            return type;
        }

    }

    template <typename T>
    EventType GetEventType() {
        static_assert(
            std::is_class_v<T>,
            "Event types must be class types."
        );

        return detail::RegisterEventType<T>();
    }

}

#endif // EVENTTYPE_HPP
