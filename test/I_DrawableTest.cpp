#include "CppUTest/TestHarness.h"

#include "I_Drawable.hpp"

TEST_GROUP(I_DrawawableTestGroup) {

    void setup() {
        
    }

    void teardown() {

    }
};

TEST(I_DrawawableTestGroup, Interface) {
    class drawTest : public BurgerEngine2D::I_Drawable {
        void Draw() const override {
            return;
        };
    };

    drawTest testObject;
    CHECK(&testObject != NULL);
}

TEST(I_DrawawableTestGroup, getZ) {

}