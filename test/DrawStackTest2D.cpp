#include "CppUTest/TestHarness.h"

#include <optional>

#include "DrawStack2D.hpp"
#include "I_Drawable.hpp"

TEST_GROUP(DrawStackTestGroup) {

    
    class drawTest : public BurgerEngine2D::I_Drawable {
        void Draw() const override {
            return;
        }
        public:
            drawTest(int depth) {
                setZDepth(depth);
            }
    };
    
    std::optional<BurgerEngine2D::DrawStack2D> testStack = std::nullopt;

    std::optional<drawTest> drawObject1 = drawTest(1);
    std::optional<drawTest> drawObject3 = drawTest(3);
    std::optional<drawTest> drawObject5 = drawTest(5);



    void setup() {
        testStack = BurgerEngine2D::DrawStack2D();
    }

    void teardown() {

    }
};

TEST(DrawStackTestGroup, Instantiate) {
    CHECK_TRUE(testObject);
}