
#include <memory>

#include "I_Event.hpp"
#include "I_Subscriber.hpp"

#include <CppUTest/TestHarness.h>

class SubscriberTestEvent : public BurgerEngine2D::I_Event {
    public:
        BurgerEngine2D::EventType getType() const noexcept override {
            return 12;
        }
};

class TestSubscriber : public BurgerEngine2D::I_Subscriber {
    public:
        std::shared_ptr<const BurgerEngine2D::I_Event> receivedEvent;

        void onEvent(
            std::shared_ptr<const BurgerEngine2D::I_Event> event
        ) override {
            receivedEvent = event;
        }
};

TEST_GROUP(SubscriberTestGroup) {
};

TEST(SubscriberTestGroup, OnEventReceivesTheEvent) {
    TestSubscriber subscriber;
    std::shared_ptr<const BurgerEngine2D::I_Event> event =
        std::make_shared<SubscriberTestEvent>();

    subscriber.onEvent(event);

    CHECK(subscriber.receivedEvent != nullptr);
    CHECK(subscriber.receivedEvent == event);
}

TEST(SubscriberTestGroup, OnEventReceivesTheCorrectEventType) {
    TestSubscriber subscriber;
    std::shared_ptr<const BurgerEngine2D::I_Event> event =
        std::make_shared<SubscriberTestEvent>();

    subscriber.onEvent(event);

    CHECK(subscriber.receivedEvent->getType() == 12);
}

TEST(SubscriberTestGroup, SubscriberCanReceiveDifferentEvents) {
    TestSubscriber subscriber;

    std::shared_ptr<const BurgerEngine2D::I_Event> firstEvent =
        std::make_shared<SubscriberTestEvent>();

    subscriber.onEvent(firstEvent);

    std::shared_ptr<const BurgerEngine2D::I_Event> secondEvent =
        std::make_shared<SubscriberTestEvent>();

    subscriber.onEvent(secondEvent);

    CHECK(subscriber.receivedEvent == secondEvent);
    CHECK(subscriber.receivedEvent != firstEvent);
}
