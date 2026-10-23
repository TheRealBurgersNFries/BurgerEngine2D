#include "EventType.hpp"

#include <limits>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

namespace BurgerEngine2D::detail {

    EventType RegisterEventType(std::type_index type) {
        static std::unordered_map<std::type_index, EventType> registry;
        static std::mutex registryMutex;
        static EventType nextType = 0;
        static bool exhausted = false;

        const std::lock_guard<std::mutex> lock(registryMutex);

        const auto existing = registry.find(type);

        if (existing != registry.end()) {
            return existing->second;
        }

        if (exhausted) {
            throw std::overflow_error("Event type identifiers exhausted.");
        }

        const auto [entry, inserted] = registry.emplace(type, nextType);

        if (inserted) {
            if (nextType == std::numeric_limits<EventType>::max()) {
                exhausted = true;
            } else {
                ++nextType;
            }
        }

        return entry->second;
    }

}