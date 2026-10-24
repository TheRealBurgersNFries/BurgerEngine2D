

#include "Dispatcher.hpp"
#include "DrawStack2D.hpp"
#include "EventType.hpp"
#include "I_Event.hpp"
#include "I_Subscriber.hpp"
#include "KeyboardController.hpp"
#include "KeyboardKey.hpp"
#include "KeyPressedEvent.hpp"
#include "Vector2.hpp"

#include "KeyboardMock.hpp"
#include "RayLibMock.hpp"

#include <memory>
#include <vector>

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

namespace {
    using Key = BurgerEngine2D::KeyboardKey;

    class RecordingKeySubscriber : public BurgerEngine2D::I_Subscriber {
    public:
        int eventCount = 0;
        std::vector<BurgerEngine2D::KeyboardKey> receivedKeys;

        void onEvent(
            std::shared_ptr<const BurgerEngine2D::I_Event> event
        ) override {
            const auto keyEvent =
                std::dynamic_pointer_cast<
                    const BurgerEngine2D::KeyPressedEvent
                >(event);

            if (!keyEvent) {
                return;
            }

            ++eventCount;
            receivedKeys.push_back(keyEvent->getData().key);
        }
    };
}



TEST_GROUP(KeyEventTestGroup) {
    BurgerEngine2D::KeyboardWrapper keyboard;
    BurgerEngine2D::KeyboardController controller{keyboard};

    static constexpr Key testKey = static_cast<Key>(32);
    static constexpr Key secondTestKey = static_cast<Key>(65);

    void setup() {
        
        // Drain pending events before resetting the test state.
        BurgerEngine2D::Dispatcher::dispatch();
        BurgerEngine2D::Dispatcher::clearAllSubscribers();

        BurgerEngine2D::mockResetKeyboard();
        BurgerEngine2D::resetMockCalls();
    }

    void teardown() {
        // Deliver any remaining queued events while test subscribers
        // are still valid, then remove all subscriptions.
        BurgerEngine2D::Dispatcher::dispatch();
        BurgerEngine2D::Dispatcher::clearAllSubscribers();

        BurgerEngine2D::mockResetKeyboard();
        
    }

    void subscribeToKeyPressed(RecordingKeySubscriber& subscriber) {
        BurgerEngine2D::Dispatcher::addSubscriber(
            BurgerEngine2D::GetEventType<
                BurgerEngine2D::KeyPressedEvent
            >(),
            &subscriber
        );
    }

    BurgerEngine2D::KeyboardKey keyboardKey(Key key) {
        return static_cast<BurgerEngine2D::KeyboardKey>(key);
    }
};

TEST(KeyEventTestGroup, heldKeyQueuesKeyPressedEvent) {
    RecordingKeySubscriber subscriber;
    subscribeToKeyPressed(subscriber);

    controller.subscribe(testKey, [](Key) {});
    BurgerEngine2D::mockPressKey(testKey);

    controller.update();

    // The controller queues events; it does not dispatch them.
    CHECK_EQUAL(0, subscriber.eventCount);

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriber.eventCount);
    CHECK_EQUAL(
        static_cast<int>(testKey),
        static_cast<int>(subscriber.receivedKeys[0])
    );
}

TEST(KeyEventTestGroup, heldKeyProducesEventOnEveryUpdateCycle) {
    RecordingKeySubscriber subscriber;
    subscribeToKeyPressed(subscriber);

    controller.subscribe(testKey, [](Key) {});
    BurgerEngine2D::mockPressKey(testKey);

    controller.update();
    BurgerEngine2D::Dispatcher::dispatch();

    controller.update();
    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(2, subscriber.eventCount);
    CHECK_EQUAL(2, subscriber.receivedKeys.size());
}

TEST(KeyEventTestGroup, releasedKeyDoesNotProduceEvent) {
    RecordingKeySubscriber subscriber;
    subscribeToKeyPressed(subscriber);
    controller.subscribe(testKey, [](Key) {});

    BurgerEngine2D::mockPressKey(testKey);
    controller.update();
    BurgerEngine2D::Dispatcher::dispatch();

    BurgerEngine2D::mockReleaseKey(testKey);
    controller.update();
    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriber.eventCount);
}

TEST(KeyEventTestGroup, unpressedRegisteredKeyProducesNoEvent) {
    RecordingKeySubscriber subscriber;
    subscribeToKeyPressed(subscriber);

    controller.subscribe(testKey, [](Key) {});

    controller.update();
    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(0, subscriber.eventCount);
}

TEST(KeyEventTestGroup, multipleHeldKeysProduceSeparateEvents) {
    RecordingKeySubscriber subscriber;
    subscribeToKeyPressed(subscriber);

    controller.subscribe(testKey, [](Key) {});
    controller.subscribe(secondTestKey, [](Key) {});

    
    BurgerEngine2D::mockPressKey(testKey);
    BurgerEngine2D::mockPressKey(secondTestKey);

    controller.update();
    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(2, subscriber.eventCount);
    CHECK_EQUAL(2, subscriber.receivedKeys.size());

    bool foundFirst = false;
    bool foundSecond = false;

    for (BurgerEngine2D::KeyboardKey key : subscriber.receivedKeys) {
        if (key == testKey) {
            foundFirst = true;
        }

        if (key == secondTestKey) {
            foundSecond = true;
        }
    }

    CHECK_TRUE(foundFirst);
    CHECK_TRUE(foundSecond);
}

TEST(KeyEventTestGroup, multipleSubscribersReceiveSameEvent) {
    RecordingKeySubscriber firstSubscriber;
    RecordingKeySubscriber secondSubscriber;

    subscribeToKeyPressed(firstSubscriber);
    subscribeToKeyPressed(secondSubscriber);

    controller.subscribe(testKey, [](Key) {});
    BurgerEngine2D::mockPressKey(testKey);

    controller.update();
    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, firstSubscriber.eventCount);
    CHECK_EQUAL(1, secondSubscriber.eventCount);
}

TEST(KeyEventTestGroup, callbackRunsEveryCycleWhileKeyIsHeld) {
    int callbackCount = 0;

    controller.subscribe(testKey, [&callbackCount](Key key) {
        CHECK_EQUAL(static_cast<int>(testKey), static_cast<int>(key));
        ++callbackCount;
    });

    BurgerEngine2D::mockPressKey(testKey);

    controller.update();
    controller.update();
    controller.update();

    CHECK_EQUAL(3, callbackCount);
}

TEST(KeyEventTestGroup, unsubscribedKeyBindingStopsInvokingCallback) {
    int callbackCount = 0;

    const std::size_t subscription = controller.subscribe(
        testKey,
        [&callbackCount](Key) {
            ++callbackCount;
        }
    );

    BurgerEngine2D::mockPressKey(testKey);

    controller.update();
    CHECK_EQUAL(1, callbackCount);

    controller.unsubscribe(subscription);

    controller.update();
    CHECK_EQUAL(1, callbackCount);
}

TEST(KeyEventTestGroup, drawStackUpdatesKeyboardControllerAfterDrawing) {
    RecordingKeySubscriber subscriber;
    subscribeToKeyPressed(subscriber);

    controller.subscribe(testKey, [](Key) {});
    BurgerEngine2D::mockPressKey(testKey);

    BurgerEngine2D::DrawStack2D drawStack;
    drawStack.setKeyboardController(&controller);

    // The draw list is empty, so no camera is required for this test.
    drawStack.Draw(BurgerEngine2D::Vector2{0.0f, 0.0f});

    CHECK_EQUAL(1, BurgerEngine2D::getBeginDrawingCalls());
    CHECK_EQUAL(1, BurgerEngine2D::getEndDrawingCalls());

    // Input update queues the event, but does not dispatch it.
    CHECK_EQUAL(0, subscriber.eventCount);

    BurgerEngine2D::Dispatcher::dispatch();

    CHECK_EQUAL(1, subscriber.eventCount);
}

TEST(KeyEventTestGroup, drawStackWithoutKeyboardControllerStillDraws) {
    BurgerEngine2D::DrawStack2D drawStack;

    drawStack.Draw(BurgerEngine2D::Vector2{0.0f, 0.0f});

    CHECK_EQUAL(1, BurgerEngine2D::getBeginDrawingCalls());
    CHECK_EQUAL(1, BurgerEngine2D::getEndDrawingCalls());
}

TEST(KeyEventTestGroup, diagnosticConstructKeyPressedEvent) {
    BurgerEngine2D::KeyPressedEvent event(
        static_cast<BurgerEngine2D::KeyboardKey>(testKey)
    );
}