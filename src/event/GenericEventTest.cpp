
#include <cstdint>

#include "EventType.hpp"
#include "GenericEvent.hpp"
#include "I_Event.hpp"

#include <CppUTest/TestHarness.h>

TEST_GROUP(GenericEventTestGroup) {
};

TEST(GenericEventTestGroup, ConstructorStoresTheEventType) {
    const BurgerEngine2D::EventType expectedType = 15;
    BurgerEngine2D::GenericEvent<int> event(expectedType, 100);

    CHECK(event.getType() == expectedType);
}

TEST(GenericEventTestGroup, GetTypeReturnsTheSuppliedType) {
    const BurgerEngine2D::EventType expectedType = 27;
    BurgerEngine2D::GenericEvent<int> event(expectedType, 200);

    CHECK(event.getType() == expectedType);
}

TEST(GenericEventTestGroup, GenericEventImplementsIEvent) {
    const BurgerEngine2D::EventType expectedType = 33;
    BurgerEngine2D::GenericEvent<int> concreteEvent(expectedType, 300);
    const BurgerEngine2D::I_Event& event = concreteEvent;

    CHECK(event.getType() == expectedType);
}

TEST(GenericEventTestGroup, SupportsDifferentPayloadTypes) {
    const BurgerEngine2D::EventType integerType = 1;
    const BurgerEngine2D::EventType floatType = 2;

    BurgerEngine2D::GenericEvent<int> integerEvent(integerType, 123);
    BurgerEngine2D::GenericEvent<float> floatEvent(floatType, 4.5F);

    CHECK(integerEvent.getType() == integerType);
    CHECK(floatEvent.getType() == floatType);
}
