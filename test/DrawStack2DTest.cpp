
#include <optional>


#include "DrawStack2D.hpp"
#include "I_Drawable.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

#include "CppUTestExt/MockSupport.h"

TEST_GROUP(DrawStackTestGroup) {

    
    class drawTest : public BurgerEngine2D::I_Drawable {
        public:
            void Draw() const override {
                    return;
                }
            drawTest() {
                    setZDepth(0); 
                }
            drawTest(int depth) {
                    setZDepth(depth);
                }
    };
    
    std::optional<BurgerEngine2D::DrawStack2D> testStack = std::nullopt;

    std::optional<drawTest> drawObject1 = std::nullopt;
    std::optional<drawTest> drawObject2 = std::nullopt;
    std::optional<drawTest> drawObject3 = std::nullopt;
    std::optional<drawTest> drawObject4 = std::nullopt;
      

    void setup() {
        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
        testStack.emplace();
    }

    void teardown() {

        mock().checkExpectations();
        mock().clear();     

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
    drawObject1 = drawTest();
    testStack->AddToStack(&*drawObject1);
    bool actual1 = drawObject1->InStack();
    bool actual2 = false;
    if (testStack->getDrawCount() > 0)
        actual2 = true;

    CHECK_TRUE(actual1 & actual2);

}

TEST(DrawStackTestGroup, keepZDepth) {
    int expected = 12;
    drawObject1 = drawTest(expected);
    testStack->AddToStack(&*drawObject1);
    int actual =  drawObject1->getZDepth();
    CHECK_EQUAL(expected, actual);
}

TEST(DrawStackTestGroup, autoZDepth) {
    drawObject1 = drawTest();
    drawObject2 = drawTest();
    drawObject3 = drawTest();
    drawObject4 = drawTest();

    testStack->AddToStack(&*drawObject1);
    testStack->AddToStack(&*drawObject2);
    testStack->AddToStack(&*drawObject3);
    testStack->AddToStack(&*drawObject4);
    
    int expected = 3;
    int actual = drawObject4->getZDepth();
    CHECK_EQUAL(expected, actual);
    
}

TEST(DrawStackTestGroup, partialAutoZDepth) {
    drawObject1 = drawTest(1);
    drawObject2 = drawTest(3);
    drawObject3 = drawTest(5);
    drawObject4 = drawTest();

    testStack->AddToStack(&*drawObject1);
    testStack->AddToStack(&*drawObject2);
    testStack->AddToStack(&*drawObject3);
    testStack->AddToStack(&*drawObject4);
    
    int expected = 6;
    int actual = drawObject4->getZDepth();
    CHECK_EQUAL(expected, actual);
}

TEST(DrawStackTestGroup, removeFromGroup) {
    drawObject1 = drawTest();
    drawObject2 = drawTest();
    drawObject3 = drawTest();
    drawObject4 = drawTest();

    testStack->AddToStack(&*drawObject1);
    testStack->AddToStack(&*drawObject2);
    testStack->AddToStack(&*drawObject3);
    testStack->AddToStack(&*drawObject4);
    
    mock().expectOneCall("beginDrawing");
    mock().expectOneCall("endDrawing");
    testStack->Draw();

    testStack->removeFromStack(&*drawObject1);

    mock().expectOneCall("beginDrawing");
    mock().expectOneCall("endDrawing");
    testStack->Draw();

    mock().checkExpectations();
    mock().clear();
    CHECK_FALSE(drawObject1->InStack());
}

TEST(DrawStackTestGroup, drawCallsRaylibFunctions) {
    mock().expectOneCall("beginDrawing");
    mock().expectOneCall("endDrawing");

    testStack->Draw();

    mock().checkExpectations();
    mock().clear();
}