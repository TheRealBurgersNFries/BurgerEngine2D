#include <cstddef>
#include <optional>

#include "Dispatcher.hpp"
#include "KeyboardController.hpp"
#include "KeyboardKey.hpp"
#include "KeyboardWrapper.hpp"
#include "KeyboardMock.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

namespace {
    using Key = BurgerEngine2D::KeyboardKey;
    using SubscriptionId = std::size_t;
}

TEST_GROUP(KeyboardControllerTestGroup) {
    BurgerEngine2D::KeyboardWrapper keyboard;
    std::optional<BurgerEngine2D::KeyboardController> controller;

    void setup() {
        BurgerEngine2D::mockResetKeyboard();
        controller.emplace(keyboard);
    }

    void teardown() {
        // Key presses queue events even when this fixture has no event
        // subscribers. Drain them here so no event is carried into another
        // test fixture through the dispatcher's static queue.
        BurgerEngine2D::Dispatcher::clearAllSubscribers();
        BurgerEngine2D::Dispatcher::dispatch();

        controller.reset();
        BurgerEngine2D::mockResetKeyboard();
        
    }
};

TEST(KeyboardControllerTestGroup, SubscribedListenerReceivesKeyEvent) {
    constexpr Key TEST_KEY = Key::A;

    int callbackCount = 0;

    controller->subscribe(TEST_KEY, [&](Key key) {
        CHECK_EQUAL(static_cast<int>(TEST_KEY),static_cast<int>(key));
        ++callbackCount;
    });

    BurgerEngine2D::mockPressKey(TEST_KEY);
    controller->update();

    CHECK_EQUAL(1, callbackCount);
}

TEST(KeyboardControllerTestGroup, SubscribeRegistersKey) {
    constexpr Key TEST_KEY = Key::A;

    controller->subscribe(TEST_KEY, [](Key) {});

    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, MultipleSubscriptionsRegisterKeyOnce) {
    constexpr Key TEST_KEY = Key::A;

    controller->subscribe(TEST_KEY, [](Key) {});
    controller->subscribe(TEST_KEY, [](Key) {});

    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, UnsubscribeRemovesOnlySpecifiedListener) {
    constexpr Key TEST_KEY = Key::A;

    int firstCount = 0;
    int secondCount = 0;

    const std::size_t firstId =
        controller->subscribe(TEST_KEY, [&](Key) {
            ++firstCount;
        });

    controller->subscribe(TEST_KEY, [&](Key) {
        ++secondCount;
    });

    controller->unsubscribe(firstId);

    BurgerEngine2D::mockPressKey(TEST_KEY);
    controller->update();

    CHECK_EQUAL(0, firstCount);
    CHECK_EQUAL(1, secondCount);
    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, LastUnsubscribeRemovesKeyFromRegistry) {
    constexpr Key TEST_KEY = Key::A;

    const std::size_t id =
        controller->subscribe(TEST_KEY, [](Key) {});

    controller->unsubscribe(id);

    controller->update();

    CHECK_EQUAL(0, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, KeyRemainsRegisteredUntilLastUnsubscribe) {
    constexpr Key TEST_KEY = Key::A;

    const SubscriptionId firstId =
        controller->subscribe(TEST_KEY, [](Key) {});

    const SubscriptionId secondId =
        controller->subscribe(TEST_KEY, [](Key) {});

    controller->unsubscribe(firstId);
    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));

    controller->unsubscribe(secondId);
    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, RepeatedUnsubscribeDoesNotBreakRegistry) {
    constexpr Key TEST_KEY = Key::A;

    int callbackCount = 0;

    const SubscriptionId firstId =
        controller->subscribe(TEST_KEY, [](Key) {});

    controller->subscribe(TEST_KEY, [&](Key) {
        ++callbackCount;
    });

    controller->unsubscribe(firstId);
    controller->unsubscribe(firstId);

    BurgerEngine2D::mockPressKey(TEST_KEY);
    controller->update();

    CHECK_EQUAL(1, callbackCount);
    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, UnknownSubscriptionIdDoesNotAffectListeners) {
    constexpr Key TEST_KEY = Key::A;

    int callbackCount = 0;

    controller->subscribe(TEST_KEY, [&](Key) {
        ++callbackCount;
    });

    controller->unsubscribe(999999);

    BurgerEngine2D::mockPressKey(TEST_KEY);
    controller->update();

    CHECK_EQUAL(1, callbackCount);
    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}
