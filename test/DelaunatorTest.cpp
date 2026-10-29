#include "Delaunator.hpp"

#include <CppUTest/TestHarness.h>

#include <vector>

using BurgerEngine2D::Delaunator;
using BurgerEngine2D::Vector2;

TEST_GROUP(DelaunatorTestGroup) {};

TEST(DelaunatorTestGroup, TriangulatesOneTriangleAndReportsItsHullArea) {
    const std::vector<Vector2> points = {
        {0.0f, 0.0f}, {4.0f, 0.0f}, {0.0f, 3.0f}
    };

    const Delaunator triangulation(points);

    CHECK_EQUAL(3, triangulation.getTriangles().size());
    CHECK_EQUAL(3, triangulation.getHalfEdges().size());
    DOUBLES_EQUAL(6.0, triangulation.getHullArea(), 0.0001);
    CHECK_TRUE(triangulation.getCoordinates().size() == points.size());
}

TEST(DelaunatorTestGroup, TriangulatesSquareIntoTwoTrianglesWithLinkedInteriorEdge) {
    const std::vector<Vector2> points = {
        {0.0f, 0.0f}, {2.0f, 0.0f}, {2.0f, 2.0f}, {0.0f, 2.0f}
    };

    const Delaunator triangulation(points);
    const std::vector<std::size_t>& triangles = triangulation.getTriangles();
    const std::vector<std::size_t>& halfEdges = triangulation.getHalfEdges();

    CHECK_EQUAL(6, triangles.size());
    CHECK_EQUAL(6, halfEdges.size());
    DOUBLES_EQUAL(4.0, triangulation.getHullArea(), 0.0001);

    std::size_t linked_edges = 0;
    for (std::size_t edge = 0; edge < halfEdges.size(); ++edge) {
        if (halfEdges[edge] != Delaunator::INVALID_INDEX) {
            ++linked_edges;
            CHECK_EQUAL(edge, halfEdges[halfEdges[edge]]);
        }
    }
    CHECK_EQUAL(2, linked_edges);
}

TEST(DelaunatorTestGroup, EmptyAndSinglePointInputsProduceNoTriangles) {
    const std::vector<Vector2> empty;
    const Delaunator emptyTriangulation(empty);
    CHECK_EQUAL(0, emptyTriangulation.getTriangles().size());
    DOUBLES_EQUAL(0.0, emptyTriangulation.getHullArea(), 0.0001);

    const std::vector<Vector2> onePoint = {{5.0f, -2.0f}};
    const Delaunator singlePointTriangulation(onePoint);
    CHECK_EQUAL(0, singlePointTriangulation.getTriangles().size());
    DOUBLES_EQUAL(0.0, singlePointTriangulation.getHullArea(), 0.0001);

    const std::vector<Vector2> twoPoints = {{-3.0f, 2.0f}, {4.0f, 2.0f}};
    const Delaunator twoPointTriangulation(twoPoints);
    CHECK_EQUAL(0, twoPointTriangulation.getTriangles().size());
    DOUBLES_EQUAL(0.0, twoPointTriangulation.getHullArea(), 0.0001);
}

TEST(DelaunatorTestGroup, DuplicateInputPointsDoNotCreateInvalidIndices) {
    const std::vector<Vector2> points = {
        {0.0f, 0.0f}, {4.0f, 0.0f}, {0.0f, 3.0f}, {0.0f, 0.0f}
    };

    const Delaunator triangulation(points);

    CHECK_EQUAL(3, triangulation.getTriangles().size());
    for (std::size_t vertex : triangulation.getTriangles()) {
        CHECK_TRUE(vertex < points.size());
    }
}

TEST(DelaunatorTestGroup, DoublePrecisionNearDuplicatesAreTreatedAsEqual) {
    const std::vector<Vector2> points = {
        {0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0}, {0.00000000000000005, 0.00000000000000005}
    };

    const Delaunator triangulation(points);

    CHECK_EQUAL(3, triangulation.getTriangles().size());
}
