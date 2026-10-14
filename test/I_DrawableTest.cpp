#include "CppUTest/TestHarness.h"

#include <optional>

#include "I_Drawable.hpp"
#include "DrawStack2D.hpp"

TEST_GROUP(I_DrawableTestGroup) {
    class drawTest : public BurgerEngine2D::I_Drawable {
        void Draw() const override {
            return;
        };
    };

    std::optional<drawTest> testObject = std::nullopt;
    std::optional<BurgerEngine2D::DrawStack2D> testStack = std::nullopt;

    void setup() {
        testObject = drawTest();
        testStack = BurgerEngine2D::DrawStack2D();
    }

    void teardown() {
        testObject.reset();
    }

};

TEST(I_DrawableTestGroup, Interface) {   
    CHECK_TRUE(testObject);
}

TEST(I_DrawableTestGroup, getZ) {
    int actual = testObject->getZDepth();
    CHECK_EQUAL_ZERO(actual);
}

TEST(I_DrawableTestGroup, setZ) {
    int val = 2;
    testObject->setZDepth(val);
    int actual = testObject->getZDepth();
    CHECK_EQUAL(val, actual);
}

TEST(I_DrawableTestGroup, inStack) {
    bool actual = testObject->InStack();
    CHECK_FALSE(actual);
}