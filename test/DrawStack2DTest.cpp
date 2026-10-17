
#include <optional>


#include "DrawStack2D.hpp"
#include "I_Drawable.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

#include "CppUTestExt/MockSupport.h"

TEST_GROUP(DrawStackTestGroup) {

    
    class drawTest : public BurgerEngine2D::I_Drawable {
        public:
            void drawSpecific([[maybe_unused]]BurgerEngine2D::Vector2 position, [[maybe_unused]]float scale) const override  {
                return;
            }

            drawTest() {
                setZDepth(0); 
            }
            
            drawTest(int depth) {
                setZDepth(depth);
            }

            BurgerEngine2D::Vector2 testOutput(BurgerEngine2D::Vector2 position, float scale) {
                return (convertPosition(position, scale));
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
    
    testStack->Draw();

    testStack->removeFromStack(&*drawObject1);

    testStack->Draw();
    
    
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
    
    testStack->Draw();

    testStack->removeFromStack(&*drawObject1);

    testStack->Draw();
    
    
    CHECK_FALSE(testStack->isInStack(&*drawObject1));
}

TEST(DrawStackTestGroup, drawCallsRaylibFunctions) {
    mock().expectOneCall("beginDrawing");
    mock().expectOneCall("endDrawing");

    testStack->Draw();

    mock().checkExpectations();
    mock().clear();
}