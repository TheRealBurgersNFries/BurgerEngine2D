#include "EventType.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

namespace {
    struct KeyboardEvent {};
    struct MouseEvent {};
    struct CollisionEvent {};
}

TEST_GROUP(EventTypeTest) {
        void setup() {
            MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
        }
        void teardown() {
            MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
        }
};

TEST(EventTypeTest, ReturnsSameIdentifierForRepeatedRequests) {
    const BurgerEngine2D::EventType first =
        BurgerEngine2D::GetEventType<KeyboardEvent>();

    const BurgerEngine2D::EventType second =
        BurgerEngine2D::GetEventType<KeyboardEvent>();

    CHECK(first == second);
}

TEST(EventTypeTest, ReturnsDifferentIdentifiersForDifferentTypes) {
    const BurgerEngine2D::EventType keyboardType =
        BurgerEngine2D::GetEventType<KeyboardEvent>();

    const BurgerEngine2D::EventType mouseType =
        BurgerEngine2D::GetEventType<MouseEvent>();

    CHECK(keyboardType != mouseType);
}

TEST(EventTypeTest, PreservesIdentifiersAcrossRequestOrder) {
    const BurgerEngine2D::EventType keyboardType =
        BurgerEngine2D::GetEventType<KeyboardEvent>();

    const BurgerEngine2D::EventType collisionType =
        BurgerEngine2D::GetEventType<CollisionEvent>();

    const BurgerEngine2D::EventType mouseType =
        BurgerEngine2D::GetEventType<MouseEvent>();

    CHECK(keyboardType == BurgerEngine2D::GetEventType<KeyboardEvent>());
    CHECK(collisionType == BurgerEngine2D::GetEventType<CollisionEvent>());
    CHECK(mouseType == BurgerEngine2D::GetEventType<MouseEvent>());
}

TEST(EventTypeTest, ReturnsDistinctIdentifiersForMultipleEventTypes) {
    const BurgerEngine2D::EventType keyboardType =
        BurgerEngine2D::GetEventType<KeyboardEvent>();

    const BurgerEngine2D::EventType mouseType =
        BurgerEngine2D::GetEventType<MouseEvent>();

    const BurgerEngine2D::EventType collisionType =
        BurgerEngine2D::GetEventType<CollisionEvent>();

    CHECK(keyboardType != mouseType);
    CHECK(keyboardType != collisionType);
    CHECK(mouseType != collisionType);
}