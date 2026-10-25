
#include "I_Shape.hpp"

#include <CppUTest/TestHarness.h>

#include <cmath>
#include <vector>

TEST_GROUP(ShapeTestGroup) {
    void setup() {}
    void teardown() {}
};

TEST(ShapeTestGroup, EmptyShapeHasZeroCenterAndRadius) {
    BurgerEngine2D::I_Shape shape({});

    DOUBLES_EQUAL(0.0, shape.getCenter().getX(), 0.0001);
    DOUBLES_EQUAL(0.0, shape.getCenter().getY(), 0.0001);
    DOUBLES_EQUAL(0.0, shape.getRadius(), 0.0001);
    CHECK_EQUAL(0, shape.getPoints().size());
}

TEST(ShapeTestGroup, SinglePointHasThatPointAsCenter) {
    std::vector<BurgerEngine2D::Vector2> points = {
        {3.0f, 4.0f}
    };

    BurgerEngine2D::I_Shape shape(points);

    DOUBLES_EQUAL(3.0, shape.getCenter().getX(), 0.0001);
    DOUBLES_EQUAL(4.0, shape.getCenter().getY(), 0.0001);
    DOUBLES_EQUAL(0.0, shape.getRadius(), 0.0001);
}

TEST(ShapeTestGroup, SymmetricPointsProduceExpectedCenterAndRadius) {
    std::vector<BurgerEngine2D::Vector2> points = {
        {-2.0f,  0.0f},
        { 2.0f,  0.0f},
        { 0.0f,  2.0f},
        { 0.0f, -2.0f}
    };

    BurgerEngine2D::I_Shape shape(points);

    DOUBLES_EQUAL(0.0, shape.getCenter().getX(), 0.0001);
    DOUBLES_EQUAL(0.0, shape.getCenter().getY(), 0.0001);
    DOUBLES_EQUAL(2.0, shape.getRadius(), 0.0001);
}

TEST(ShapeTestGroup, AsymmetricPointsProduceArithmeticMeanCenter) {
    std::vector<BurgerEngine2D::Vector2> points = {
        {0.0f, 0.0f},
        {6.0f, 0.0f},
        {0.0f, 3.0f}
    };

    BurgerEngine2D::I_Shape shape(points);

    DOUBLES_EQUAL(2.0, shape.getCenter().getX(), 0.0001);
    DOUBLES_EQUAL(1.0, shape.getCenter().getY(), 0.0001);
    DOUBLES_EQUAL(std::sqrt(17.0), shape.getRadius(), 0.0001);
}

TEST(ShapeTestGroup, NegativeCoordinatesAreHandledCorrectly) {
    std::vector<BurgerEngine2D::Vector2> points = {
        {-4.0f, -2.0f},
        {-2.0f, -2.0f}
    };

    BurgerEngine2D::I_Shape shape(points);

    DOUBLES_EQUAL(-3.0, shape.getCenter().getX(), 0.0001);
    DOUBLES_EQUAL(-2.0, shape.getCenter().getY(), 0.0001);
    DOUBLES_EQUAL(1.0, shape.getRadius(), 0.0001);
}

TEST(ShapeTestGroup, CoincidentPointsProduceZeroRadius) {
    std::vector<BurgerEngine2D::Vector2> points = {
        {5.0f, 5.0f},
        {5.0f, 5.0f},
        {5.0f, 5.0f}
    };

    BurgerEngine2D::I_Shape shape(points);

    DOUBLES_EQUAL(5.0, shape.getCenter().getX(), 0.0001);
    DOUBLES_EQUAL(5.0, shape.getCenter().getY(), 0.0001);
    DOUBLES_EQUAL(0.0, shape.getRadius(), 0.0001);
}

TEST(ShapeTestGroup, PointsAreStoredInTheShape) {
    std::vector<BurgerEngine2D::Vector2> points = {
        {1.0f, 2.0f},
        {3.0f, 4.0f},
        {5.0f, 6.0f}
    };

    BurgerEngine2D::I_Shape shape(points);

    CHECK_EQUAL(3, shape.getPoints().size());

    DOUBLES_EQUAL(1.0, shape.getPoints()[0].getX(), 0.0001);
    DOUBLES_EQUAL(2.0, shape.getPoints()[0].getY(), 0.0001);
    DOUBLES_EQUAL(5.0, shape.getPoints()[2].getX(), 0.0001);
    DOUBLES_EQUAL(6.0, shape.getPoints()[2].getY(), 0.0001);
}

TEST(ShapeTestGroup, ModifyingInputDoesNotModifyStoredPoints) {
    std::vector<BurgerEngine2D::Vector2> points = {
        {1.0f, 2.0f},
        {3.0f, 4.0f}
    };

    BurgerEngine2D::I_Shape shape(points);

    points.clear();

    CHECK_EQUAL(2, shape.getPoints().size());
    DOUBLES_EQUAL(2.0, shape.getCenter().getX(), 0.0001);
    DOUBLES_EQUAL(3.0, shape.getCenter().getY(), 0.0001);
    DOUBLES_EQUAL(std::sqrt(2.0), shape.getRadius(), 0.0001);
}
