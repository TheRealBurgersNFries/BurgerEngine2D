
#include <optional>

#include "I_Drawable.hpp"
#include "DrawStack2D.hpp"
#include "Camera2D.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

TEST_GROUP(I_DrawableTestGroup) {
    class drawTest : public BurgerEngine2D::I_Drawable {
        public: 
            void drawSpecific([[maybe_unused]]BurgerEngine2D::Vector2 pos, [[maybe_unused]]float scale) const override {
                    return;
                }

            drawTest() {
                    setZDepth(0); 
                }

            drawTest(int depth) {
                    setZDepth(depth);
                }

            BurgerEngine2D::Vector2 testOutput(BurgerEngine2D::Vector2 position, float scale, BurgerEngine2D::Vector2 offset) {
                return (convertPosition(position, scale, offset));
            }
            
    };

    std::optional<drawTest> testObject = std::nullopt;
    std::optional<BurgerEngine2D::DrawStack2D> testStack = std::nullopt;
    BurgerEngine2D::Camera2D camera = BurgerEngine2D::Camera2D();

    void setup() {
        testObject = drawTest();
        testStack.emplace();
        testStack->setCamera(&camera);
    }

    void teardown() {
        testObject.reset();
    }

};

TEST(I_DrawableTestGroup, interface) {   
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

TEST(I_DrawableTestGroup, setPosition) {
    BurgerEngine2D::Vector2 expected(4.0f, 8.0f);
    
    testObject->setPosition(expected);
    auto actual = testObject->getPosition();
    CHECK_TRUE(actual == expected);

}

TEST(I_DrawableTestGroup, convertPosition) {
    BurgerEngine2D::Vector2 camPosition(-400.0f,-200.0f);
    float scale = 0.5f;
    BurgerEngine2D::Vector2 expected(350,400);
    BurgerEngine2D::Vector2 position(300,600);

    testObject->setPosition(position);
    auto actual = testObject->testOutput(camPosition, scale, BurgerEngine2D::Vector2(0.0f, 0.0f));

    CHECK_TRUE(actual == expected);

}