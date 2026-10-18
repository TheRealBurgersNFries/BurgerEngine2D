#include <optional>

#include "KeyboardController.hpp"
#include "wrappers/KeyboardWrapper.hpp"
#include "mocks/KeyboardMock.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

TEST_GROUP(KeyboardControllerTestGroup) {
    BurgerEngine2D::KeyboardWrapper keyboard;
    std::optional<BurgerEngine2D::KeyboardController> controller;

    void setup() {
        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
        BurgerEngine2D::mockResetKeyboard();
        controller.emplace(keyboard);
    }

    void teardown() {
        controller.reset();
        BurgerEngine2D::mockResetKeyboard();
        MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    }
};

TEST(KeyboardControllerTestGroup, SubscribedListenerReceivesKeyEvent) {
    constexpr int TEST_KEY = 65;

    int callbackCount = 0;

    controller->subscribe(TEST_KEY, [&](int key) {
        CHECK_EQUAL(TEST_KEY, key);
        ++callbackCount;
    });

    BurgerEngine2D::mockPressKey(TEST_KEY);
    controller->update();

    CHECK_EQUAL(1, callbackCount);
}

TEST(KeyboardControllerTestGroup, SubscribeRegistersKey) {
    constexpr int TEST_KEY = 65;

    controller->subscribe(TEST_KEY, [](int) {});

    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}


TEST(KeyboardControllerTestGroup, MultipleSubscriptionsRegisterKeyOnce) {
    constexpr int TEST_KEY = 65;

    controller->subscribe(TEST_KEY, [](int) {});
    controller->subscribe(TEST_KEY, [](int) {});

    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, UnsubscribeRemovesOnlySpecifiedListener) {
    constexpr int TEST_KEY = 65;

    int firstCount = 0;
    int secondCount = 0;

    const BurgerEngine2D::SubscriptionId firstId =
        controller->subscribe(TEST_KEY, [&](int) {
            ++firstCount;
        });

    controller->subscribe(TEST_KEY, [&](int) {
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
    constexpr int TEST_KEY = 65;

    const BurgerEngine2D::SubscriptionId id =
        controller->subscribe(TEST_KEY, [](int) {});

    controller->unsubscribe(id);

    controller->update();

    CHECK_EQUAL(0, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, KeyRemainsRegisteredUntilLastUnsubscribe) {
    constexpr int TEST_KEY = 65;

    const BurgerEngine2D::SubscriptionId firstId =
        controller->subscribe(TEST_KEY, [](int) {});

    const BurgerEngine2D::SubscriptionId secondId =
        controller->subscribe(TEST_KEY, [](int) {});

    controller->unsubscribe(firstId);
    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));

    controller->unsubscribe(secondId);
    controller->update();

    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}

TEST(KeyboardControllerTestGroup, RepeatedUnsubscribeDoesNotBreakRegistry) {
    constexpr int TEST_KEY = 65;

    int callbackCount = 0;

    const BurgerEngine2D::SubscriptionId firstId =
        controller->subscribe(TEST_KEY, [](int) {});

    controller->subscribe(TEST_KEY, [&](int) {
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
    constexpr int TEST_KEY = 65;

    int callbackCount = 0;

    controller->subscribe(TEST_KEY, [&](int) {
        ++callbackCount;
    });

    controller->unsubscribe(999999);

    BurgerEngine2D::mockPressKey(TEST_KEY);
    controller->update();

    CHECK_EQUAL(1, callbackCount);
    CHECK_EQUAL(1, BurgerEngine2D::mockGetQueryCount(TEST_KEY));
}