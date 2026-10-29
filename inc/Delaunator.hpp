#ifndef BE2D_DELAUNATOR_HPP
#define BE2D_DELAUNATOR_HPP

#include <cstddef>
#include <vector>

#include "Vector2.hpp"

namespace BurgerEngine2D {

    /**
     * Delaunay triangulation of a set of two-dimensional points.
     *
     * The constructor retains a reference to its input, so that vector must
     * remain alive for the lifetime of this object. Triangle entries are
     * indices into that vector, stored three per triangle. Half-edge entry e
     * corresponds to the directed edge beginning at triangle entry e. It
     * contains the matching edge index, or INVALID_INDEX for a hull edge.
     */
    class Delaunator final {
    public:
        static constexpr std::size_t INVALID_INDEX = static_cast<std::size_t>(-1);

        explicit Delaunator(const std::vector<Vector2>& coordinates);

        /** Returns the signed area of the convex hull. */
        double getHullArea() const;

        /** Read-only access to the points and generated topology. */
        const std::vector<Vector2>& getCoordinates() const;
        const std::vector<std::size_t>& getTriangles() const;
        const std::vector<std::size_t>& getHalfEdges() const;
        const std::vector<std::size_t>& getHullPrevious() const;
        const std::vector<std::size_t>& getHullNext() const;
        const std::vector<std::size_t>& getTriangleHull() const;
        std::size_t getHullStart() const;

    private:
        // Keep the caller's Vector2 points as the sole point storage.
        const std::vector<Vector2>& _coordinates;
        std::vector<std::size_t> _triangles;
        std::vector<std::size_t> _halfEdges;
        std::vector<std::size_t> _previousHull;
        std::vector<std::size_t> _nextHull;
        std::vector<std::size_t> _triangleHull;
        std::size_t _initialHull;

        std::vector<std::size_t> _hash;
        Vector2 _center;
        std::size_t _hashSize;
        std::vector<std::size_t> _edgeStack;

        /** Repairs a locally non-Delaunay edge by flipping it when needed. */
        std::size_t legalize(std::size_t edge);
        /** Maps a point to the advancing-hull lookup table. */
        std::size_t hashKey(const Vector2& point) const;
        /** Appends a triangle and connects its edges to existing neighbors. */
        std::size_t addTriangle(
            std::size_t first,
            std::size_t second,
            std::size_t third,
            std::size_t firstEdge,
            std::size_t secondEdge,
            std::size_t thirdEdge);
        /** Connects two opposite half-edges. */
        void link(std::size_t firstEdge, std::size_t secondEdge);
    };

}

#endif // BE2D_DELAUNATOR_HPP
