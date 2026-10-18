
#include <optional>


#include "DrawStack2D.hpp"
#include "I_Drawable.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

#include "CppUTestExt/MockSupport.h"

TEST_GROUP(DrawStackTestGroup) {

    
    class drawTest : public BurgerEngine2D::I_Drawable {
        public:
            int _testId = 0;
            std::vector<int>* _drawOrder = nullptr;
            void drawSpecific(
                [[maybe_unused]] BurgerEngine2D::Vector2 position,
                [[maybe_unused]] float scale
            ) const override {
                if (_drawOrder != nullptr) {
                    _drawOrder->push_back(_testId);
                }
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

            void setDrawTracking(int id, std::vector<int>* drawOrder) {
                _testId = id;
                _drawOrder = drawOrder;
            }
        };
            
    
    std::optional<BurgerEngine2D::DrawStack2D> testStack = std::nullopt;

    std::optional<drawTest> drawObject1 = std::nullopt;
    std::optional<drawTest> drawObject2 = std::nullopt;
    std::optional<drawTest> drawObject3 = std::nullopt;
    std::optional<drawTest> drawObject4 = std::nullopt;
    BurgerEngine2D::Camera2D camera = BurgerEngine2D::Camera2D();

    void setup() {
        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
        testStack.emplace();
        testStack->setCamera(&camera);
    }

    void teardown() {
        
        
        testStack.reset();
        drawObject1.reset();
        drawObject2.reset();
        drawObject3.reset();
        drawObject4.reset();
        MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    }
};

TEST(DrawStackTestGroup, instantiate) {
    bool actual = testStack.has_value();
    CHECK_TRUE(actual);
}

TEST(DrawStackTestGroup, addToStack) {
    drawObject1.emplace();
    testStack->addToStack(&*drawObject1);

    CHECK_TRUE(testStack->isInStack(&*drawObject1));

}

TEST(DrawStackTestGroup, keepZDepth) {
    int expected = 12;
    drawObject1.emplace(expected);
    testStack->addToStack(&*drawObject1);
    int actual =  drawObject1->getZDepth();
    CHECK_EQUAL(expected, actual);
}

TEST(DrawStackTestGroup, autoZDepth) {

    drawObject1.emplace();
    drawObject2.emplace();
    drawObject3.emplace();
    drawObject4.emplace();

    testStack->addToStack(&*drawObject1);
    testStack->addToStack(&*drawObject2);
    testStack->addToStack(&*drawObject3);
    testStack->addToStack(&*drawObject4);
    
    int expected = 3;
    int actual = drawObject4->getZDepth();
    CHECK_EQUAL(expected, actual);
    
}

TEST(DrawStackTestGroup, partialAutoZDepth) {
    drawObject1.emplace(1);
    drawObject2.emplace(3);
    drawObject3.emplace(5);
    drawObject4.emplace();

    testStack->addToStack(&*drawObject1);
    testStack->addToStack(&*drawObject2);
    testStack->addToStack(&*drawObject3);
    testStack->addToStack(&*drawObject4);
    
    int expected = 6;
    int actual = drawObject4->getZDepth();
    CHECK_EQUAL(expected, actual);
}

TEST(DrawStackTestGroup, checkDrawCount) {

    mock().ignoreOtherCalls();

    drawObject1.emplace();
    drawObject2.emplace();
    drawObject3.emplace();
    drawObject4.emplace();

    testStack->addToStack(&*drawObject1);
    testStack->addToStack(&*drawObject2);
    testStack->addToStack(&*drawObject3);
    testStack->addToStack(&*drawObject4);
    
    testStack->Draw(BurgerEngine2D::Vector2(0.0f,0.0f));

    testStack->removeFromStack(&*drawObject1);

    testStack->Draw(BurgerEngine2D::Vector2(0.0f,0.0f));
    
    
    CHECK_EQUAL(3, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, removeFromGroup) {

    mock().ignoreOtherCalls();

    drawObject1.emplace();
    drawObject2.emplace();
    drawObject3.emplace();
    drawObject4.emplace();

    testStack->addToStack(&*drawObject1);
    testStack->addToStack(&*drawObject2);
    testStack->addToStack(&*drawObject3);
    testStack->addToStack(&*drawObject4);
    
    testStack->Draw(BurgerEngine2D::Vector2(0.0f,0.0f));

    testStack->removeFromStack(&*drawObject1);

    testStack->Draw(BurgerEngine2D::Vector2(0.0f,0.0f));
    
    
    CHECK_FALSE(testStack->isInStack(&*drawObject1));
}

TEST(DrawStackTestGroup, drawCallsRaylibFunctions) {
    mock().expectOneCall("beginDrawing");
    mock().expectOneCall("endDrawing");

    testStack->Draw(BurgerEngine2D::Vector2(0.0f,0.0f));

    mock().checkExpectations();
    mock().clear();
}

TEST(DrawStackTestGroup, duplicateAdditionIsRejected) {
    drawObject1.emplace();

    CHECK_TRUE(testStack->addToStack(&*drawObject1));
    CHECK_FALSE(testStack->addToStack(&*drawObject1));

    CHECK_EQUAL(1, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, removingUnknownDrawableDoesNothing) {
    drawObject1.emplace();

    testStack->removeFromStack(&*drawObject1);

    CHECK_FALSE(testStack->isInStack(&*drawObject1));
    CHECK_EQUAL(0, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, duplicateRemovalIsIgnored) {
    mock().ignoreOtherCalls();
    drawObject1.emplace();

    testStack->addToStack(&*drawObject1);
    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    testStack->removeFromStack(&*drawObject1);
    testStack->removeFromStack(&*drawObject1);

    CHECK_EQUAL(0, testStack->getDrawCount());

    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    CHECK_FALSE(testStack->isInStack(&*drawObject1));
    CHECK_EQUAL(0, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, removesMultipleDrawables) {
    mock().ignoreOtherCalls();
    drawObject1.emplace(1);
    drawObject2.emplace(2);
    drawObject3.emplace(3);
    drawObject4.emplace(4);

    testStack->addToStack(&*drawObject1);
    testStack->addToStack(&*drawObject2);
    testStack->addToStack(&*drawObject3);
    testStack->addToStack(&*drawObject4);

    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    testStack->removeFromStack(&*drawObject1);
    testStack->removeFromStack(&*drawObject3);
    testStack->removeFromStack(&*drawObject4);

    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    CHECK_FALSE(testStack->isInStack(&*drawObject1));
    CHECK_TRUE(testStack->isInStack(&*drawObject2));
    CHECK_FALSE(testStack->isInStack(&*drawObject3));
    CHECK_FALSE(testStack->isInStack(&*drawObject4));

    CHECK_EQUAL(1, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, removingDrawableDoesNotRemoveAnotherAtSameDepth) {
    mock().ignoreOtherCalls();

    drawObject1.emplace(5);
    drawObject2.emplace(5);

    testStack->addToStack(&*drawObject1);
    testStack->addToStack(&*drawObject2);

    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    testStack->removeFromStack(&*drawObject1);
    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    CHECK_FALSE(testStack->isInStack(&*drawObject1));
    CHECK_TRUE(testStack->isInStack(&*drawObject2));
    CHECK_EQUAL(1, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, removingPendingAdditionCancelsIt) {
    mock().ignoreOtherCalls();
    drawObject1.emplace(1);

    testStack->addToStack(&*drawObject1);
    CHECK_TRUE(testStack->isInStack(&*drawObject1));
    CHECK_EQUAL(1, testStack->getDrawCount());

    testStack->removeFromStack(&*drawObject1);

    CHECK_FALSE(testStack->isInStack(&*drawObject1));
    CHECK_EQUAL(0, testStack->getDrawCount());

    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    CHECK_FALSE(testStack->isInStack(&*drawObject1));
    CHECK_EQUAL(0, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, drawingEmptyStackCallsRaylibWrappers) {
    mock().expectOneCall("beginDrawing");
    mock().expectOneCall("endDrawing");

    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    mock().checkExpectations();
    mock().clear();

    CHECK_EQUAL(0, testStack->getDrawCount());
}

TEST(DrawStackTestGroup, drawsInAscendingZDepthOrder) {
    mock().ignoreOtherCalls();
    std::vector<int> drawOrder;

    drawObject1.emplace(30);
    drawObject2.emplace(10);
    drawObject3.emplace(20);

    drawObject1->setDrawTracking(1, &drawOrder);
    drawObject2->setDrawTracking(2, &drawOrder);
    drawObject3->setDrawTracking(3, &drawOrder);

    testStack->addToStack(&*drawObject1);
    testStack->addToStack(&*drawObject2);
    testStack->addToStack(&*drawObject3);

    testStack->Draw(BurgerEngine2D::Vector2(0.0f, 0.0f));

    CHECK_EQUAL(3, static_cast<int>(drawOrder.size()));
    CHECK_EQUAL(2, drawOrder[0]);
    CHECK_EQUAL(3, drawOrder[1]);
    CHECK_EQUAL(1, drawOrder[2]);
}