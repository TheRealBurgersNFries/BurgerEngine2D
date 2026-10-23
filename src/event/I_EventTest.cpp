
#include <cstdint>

#include "I_Event.hpp"

#include <CppUTest/TestHarness.h>

class TestEvent : public BurgerEngine2D::I_Event {
    public:
        explicit TestEvent(BurgerEngine2D::EventType type) : _type(type) {}

        BurgerEngine2D::EventType getType() const noexcept override {
            return _type;
        }

    private:
        BurgerEngine2D::EventType _type;
};

TEST_GROUP(EventTestGroup) {
};

TEST(EventTestGroup, GetTypeReturnsTheEventType) {
    const BurgerEngine2D::EventType expectedType = 42;
    TestEvent event(expectedType);

    CHECK(event.getType() == expectedType);
}

TEST(EventTestGroup, EventCanBeAccessedThroughInterface) {
    const BurgerEngine2D::EventType expectedType = 7;
    TestEvent concreteEvent(expectedType);
    const BurgerEngine2D::I_Event& event = concreteEvent;

    CHECK(event.getType() == expectedType);
}

TEST(EventTestGroup, DestructorIsVirtual) {
    class DestructionTestEvent : public BurgerEngine2D::I_Event {
        public:
            explicit DestructionTestEvent(bool& destroyed)
                : _destroyed(destroyed) {}

            ~DestructionTestEvent() override {
                _destroyed = true;
            }

            BurgerEngine2D::EventType getType() const noexcept override {
                return 1;
            }

        private:
            bool& _destroyed;
    };

    bool destroyed = false;
    BurgerEngine2D::I_Event* event =
        new DestructionTestEvent(destroyed);

    delete event;

    CHECK(destroyed);
}
