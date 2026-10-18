
#include <optional>

#include "I_Drawable.hpp"
#include "DrawStack2D.hpp"
#include "Camera2D.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

TEST_GROUP(I_DrawableTestGroup) {
    class drawTest : public BurgerEngine2D::I_Drawable {
        public:
            mutable bool wasDrawn = false;
            mutable BurgerEngine2D::Vector2 lastPosition;
            mutable float lastScale = 0.0f;

            void drawSpecific(
                BurgerEngine2D::Vector2 pos,
                float scale
            ) const override {
                wasDrawn = true;
                lastPosition = pos;
                lastScale = scale;
            }

            drawTest() {
                setZDepth(0);
            }

            drawTest(int depth) {
                setZDepth(depth);
            }

            BurgerEngine2D::Vector2 testOutput(
                BurgerEngine2D::Vector2 position,
                float scale,
                BurgerEngine2D::Vector2 offset
            ) {
                return convertPosition(position, scale, offset);
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

TEST(I_DrawableTestGroup, drawCallsDrawSpecificWithConvertedPosition) {
    BurgerEngine2D::Vector2 worldPosition(300.0f, 600.0f);
    BurgerEngine2D::Vector2 cameraPosition(-400.0f, -200.0f);
    BurgerEngine2D::Vector2 offset(0.0f, 0.0f);
    float scale = 0.5f;

    testObject->setPosition(worldPosition);

    testObject->Draw(cameraPosition, scale, offset);

    CHECK_TRUE(testObject->wasDrawn);
    CHECK_TRUE(
        testObject->lastPosition ==
        BurgerEngine2D::Vector2(350.0f, 400.0f)
    );
    CHECK_EQUAL(scale, testObject->lastScale);
}

TEST(I_DrawableTestGroup, convertPositionWithOffset) {
    testObject->setPosition(BurgerEngine2D::Vector2(300.0f, 600.0f));

    BurgerEngine2D::Vector2 cameraPosition(-400.0f, -200.0f);
    BurgerEngine2D::Vector2 offset(10.0f, 20.0f);

    BurgerEngine2D::Vector2 expected(340.0f, 380.0f);

    auto actual = testObject->testOutput(
        cameraPosition,
        0.5f,
        offset
    );

    CHECK_TRUE(actual == expected);
}

TEST(I_DrawableTestGroup, convertPositionWithUnitScale) {
    testObject->setPosition(BurgerEngine2D::Vector2(100.0f, 200.0f));

    BurgerEngine2D::Vector2 cameraPosition(25.0f, 50.0f);
    BurgerEngine2D::Vector2 expected(75.0f, 150.0f);

    auto actual = testObject->testOutput(
        cameraPosition,
        1.0f,
        BurgerEngine2D::Vector2(0.0f, 0.0f)
    );

    CHECK_TRUE(actual == expected);
}