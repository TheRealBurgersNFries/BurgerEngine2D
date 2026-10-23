

#include <functional>
#include <memory>
#include <vector>


#include "Dispatcher.hpp"
#include "EventType.hpp"
#include "I_Event.hpp"
#include "I_Subscriber.hpp"


#include "CppUTest/TestHarness.h"

namespace {

const BurgerEngine2D::EventType TEST_EVENT_A =
    static_cast<BurgerEngine2D::EventType>(1);

const BurgerEngine2D::EventType TEST_EVENT_B =
    static_cast<BurgerEngine2D::EventType>(2);

class TestEvent : public BurgerEngine2D::I_Event {
private:
    BurgerEngine2D::EventType _type;

public:
    explicit TestEvent(BurgerEngine2D::EventType type)
        : _type(type) {}

    BurgerEngine2D::EventType getType() const noexcept override {
        return _type;
    }
};

class TestSubscriber : public BurgerEngine2D::I_Subscriber {
public:
    std::vector<BurgerEngine2D::EventType> receivedEvents;
    std::function<void()> callback;

    void onEvent(
        std::shared_ptr<const BurgerEngine2D::I_Event> event
    ) override {
        receivedEvents.push_back(event->getType());

        if (callback) {
            callback();
        }
    }

    std::size_t getEventCount() const {
        return receivedEvents.size();
    }
};

std::shared_ptr<const BurgerEngine2D::I_Event> createEvent(
    BurgerEngine2D::EventType type
) {
    return std::make_shared<TestEvent>(type);
}

}

TEST_GROUP(DispatcherTestGroup) {
    void setup() override {
        BurgerEngine2D::Dispatcher::clearAllSubscribers();

        // Drain events left in the static queue by earlier tests.
        BurgerEngine2D::Dispatcher::dispatch();
    }

    void teardown() override {
        // Remove subscribers before draining queued events.
        BurgerEngine2D::Dispatcher::clearAllSubscribers();
        BurgerEngine2D::Dispatcher::dispatch();
    }
};

TEST(DispatcherTestGroup, DoesNotDispatchWhenQueueIsEmpty) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(0, subscriber.getEventCount());
}

TEST(DispatcherTestGroup, QueueEventDoesNotDispatchImmediately) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    CHECK_EQUAL(0, subscriber.getEventCount());
}

TEST(DispatcherTestGroup, DispatchDeliversQueuedEvent) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriber.getEventCount());
    CHECK_TRUE(subscriber.receivedEvents[0] == TEST_EVENT_A);
}

TEST(DispatcherTestGroup, DispatchRoutesOnlyToMatchingSubscribers) {
    TestSubscriber subscriberA;
    TestSubscriber subscriberB;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriberA
    );

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_B,
        &subscriberB
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriberA.getEventCount());
    CHECK_EQUAL(0, subscriberB.getEventCount());
}

TEST(DispatcherTestGroup, DispatchDeliversMultipleQueuedEvents) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(2, subscriber.getEventCount());

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(2, subscriber.getEventCount());
}

TEST(DispatcherTestGroup, EventsQueuedDuringDispatchWaitUntilNextDispatch) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_B,
        &subscriber
    );

    subscriber.callback = [&subscriber]() {
        if (subscriber.receivedEvents.back() == TEST_EVENT_A) {
            subscriber.callback = nullptr;

            BurgerEngine2D::Dispatcher::queueEvent(
                createEvent(TEST_EVENT_B)
            );
        }
    };

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriber.getEventCount());
    CHECK_TRUE(subscriber.receivedEvents[0] == TEST_EVENT_A);

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(2, subscriber.getEventCount());
    CHECK_TRUE(subscriber.receivedEvents[1] == TEST_EVENT_B);
}

TEST(DispatcherTestGroup, AddSubscriberDoesNotCreateDuplicates) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriber.getEventCount());
}

TEST(DispatcherTestGroup, AddSubscriberIgnoresNullPointer) {
    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        nullptr
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    // Must not crash when dispatching with a null subscriber registration.
    BurgerEngine2D::Dispatcher::dispatch();
}

TEST(DispatcherTestGroup, RemoveSubscriptionRemovesOnlySpecifiedType) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_B,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::removeSubscription(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_B)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriber.getEventCount());
    CHECK_TRUE(subscriber.receivedEvents[0] == TEST_EVENT_B);
}

TEST(DispatcherTestGroup, RemoveSubscriberRemovesAllSubscriptions) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_B,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::removeSubscriber(&subscriber);

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_B)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(0, subscriber.getEventCount());
}

TEST(DispatcherTestGroup, ClearEventRemovesSubscribersForOneType) {
    TestSubscriber subscriberA;
    TestSubscriber subscriberB;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriberA
    );

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_B,
        &subscriberB
    );

    BurgerEngine2D::Dispatcher::clearEvent(TEST_EVENT_A);

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_B)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(0, subscriberA.getEventCount());
    CHECK_EQUAL(1, subscriberB.getEventCount());
}

TEST(DispatcherTestGroup, ClearAllSubscribersRemovesEverySubscription) {
    TestSubscriber subscriberA;
    TestSubscriber subscriberB;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriberA
    );

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_B,
        &subscriberB
    );

    BurgerEngine2D::Dispatcher::clearAllSubscribers();

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_A)
    );

    BurgerEngine2D::Dispatcher::queueEvent(
        createEvent(TEST_EVENT_B)
    );

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(0, subscriberA.getEventCount());
    CHECK_EQUAL(0, subscriberB.getEventCount());
}

TEST(DispatcherTestGroup, QueueEventIgnoresNullPointer) {
    TestSubscriber subscriber;

    BurgerEngine2D::Dispatcher::addSubscriber(
        TEST_EVENT_A,
        &subscriber
    );

    BurgerEngine2D::Dispatcher::queueEvent(nullptr);
    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(0, subscriber.getEventCount());
}
