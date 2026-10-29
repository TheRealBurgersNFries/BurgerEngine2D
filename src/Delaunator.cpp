#include "Delaunator.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

namespace BurgerEngine2D {
namespace {

// Vector2 stores double coordinates, so comparisons use double precision.
constexpr double EPSILON = std::numeric_limits<double>::epsilon();
constexpr std::size_t INVALID_INDEX = Delaunator::INVALID_INDEX;

//@see https://stackoverflow.com/questions/33333363/built-in-mod-vs-custom-mod-function-improve-the-performance-of-modulus-op/33333636#33333636
inline size_t fast_mod(const size_t i, const size_t c) {
    return i >= c ? i % c : i;
}

// Kahan and Babuska summation, Neumaier variant; accumulates less FP error
inline double sum(const std::vector<double>& x) {
    double sum = x[0];
    double err = 0.0;

    for (size_t i = 1; i < x.size(); i++) {
        const double k = x[i];
        const double m = sum + k;
        err += std::fabs(sum) >= std::fabs(k) ? sum - m + k : k - m + sum;
        sum = m;
    }
    return sum + err;
}

inline double dist(const Vector2& first, const Vector2& second) {
    return first.distanceSquared(second);
}

inline double circumradius(const Vector2& a, const Vector2& b, const Vector2& c) {
    const Vector2 firstEdge = b - a;
    const Vector2 secondEdge = c - a;
    const double firstLengthSquared = firstEdge.lengthSquared();
    const double secondLengthSquared = secondEdge.lengthSquared();
    const double determinant = firstEdge.cross(secondEdge);

    if (firstLengthSquared == 0.0 || secondLengthSquared == 0.0 ||
        determinant == 0.0) {
        return std::numeric_limits<double>::max();
    }

    const Vector2 centerOffset(
        (secondEdge.getY() * firstLengthSquared -
         firstEdge.getY() * secondLengthSquared) * 0.5 / determinant,
        (firstEdge.getX() * secondLengthSquared -
         secondEdge.getX() * firstLengthSquared) * 0.5 / determinant);
    return centerOffset.lengthSquared();
}

inline bool orient(const Vector2& p, const Vector2& q, const Vector2& r) {
    return (q - p).cross(r - p) > 0.0;
}

inline Vector2 circumcenter(
    const Vector2& a, const Vector2& b, const Vector2& c) {
    const Vector2 firstEdge = b - a;
    const Vector2 secondEdge = c - a;
    const double firstLengthSquared = firstEdge.lengthSquared();
    const double secondLengthSquared = secondEdge.lengthSquared();
    const double determinant = firstEdge.cross(secondEdge);
    const Vector2 centerOffset(
        (secondEdge.getY() * firstLengthSquared -
         firstEdge.getY() * secondLengthSquared) * 0.5 / determinant,
        (firstEdge.getX() * secondLengthSquared -
         secondEdge.getX() * firstLengthSquared) * 0.5 / determinant);
    return a + centerOffset;
}

struct compare {

    const std::vector<Vector2>& coordinates;
    Vector2 center;

    bool operator()(std::size_t i, std::size_t j) {
        const double d1 = dist(coordinates[i], center);
        const double d2 = dist(coordinates[j], center);
        const double diff1 = d1 - d2;
        const double diff2 = coordinates[i].getX() - coordinates[j].getX();
        const double diff3 = coordinates[i].getY() - coordinates[j].getY();

        if (diff1 > 0.0 || diff1 < 0.0) {
            return diff1 < 0;
        } else if (diff2 > 0.0 || diff2 < 0.0) {
            return diff2 < 0;
        } else {
            return diff3 < 0;
        }
    }
};

inline bool in_circle(const Vector2& a, const Vector2& b,
                      const Vector2& c, const Vector2& p) {
    const Vector2 firstOffset = a - p;
    const Vector2 secondOffset = b - p;
    const Vector2 thirdOffset = c - p;
    const double firstLengthSquared = firstOffset.lengthSquared();
    const double secondLengthSquared = secondOffset.lengthSquared();
    const double thirdLengthSquared = thirdOffset.lengthSquared();

    return firstLengthSquared * secondOffset.cross(thirdOffset) -
           secondLengthSquared * firstOffset.cross(thirdOffset) +
           thirdLengthSquared * firstOffset.cross(secondOffset) < 0.0;
}

inline bool check_pts_equal(const Vector2& a, const Vector2& b) {
    return std::fabs(static_cast<double>(a.getX()) - b.getX()) <= EPSILON &&
           std::fabs(static_cast<double>(a.getY()) - b.getY()) <= EPSILON;
}

// monotonically increases with real angle, but doesn't need expensive trigonometry
inline double pseudo_angle(const Vector2& offset) {
    const double dx = offset.getX();
    const double dy = offset.getY();
    const double p = dx / (std::abs(dx) + std::abs(dy));
    return (dy > 0.0 ? 3.0 - p : 1.0 + p) / 4.0; // [0..1)
}


} // namespace

Delaunator::Delaunator(const std::vector<Vector2>& in_coordinates)
    : _coordinates(in_coordinates),
      _triangles(),
      _halfEdges(),
      _previousHull(),
      _nextHull(),
      _triangleHull(),
      _initialHull(),
      _hash(),
      _center(),
      _hashSize(),
      _edgeStack() {
    std::size_t n = _coordinates.size();

    // Small sets do not need the advancing-hull algorithm. Keeping the
    // topology arrays aligned with the input makes their access predictable.
    if (n < 3) {
        _previousHull.assign(n, INVALID_INDEX);
        _nextHull.assign(n, INVALID_INDEX);
        _triangleHull.assign(n, INVALID_INDEX);
        _initialHull = n < 2 ? INVALID_INDEX : 0;
        if (n == 2) {
            _previousHull[0] = _nextHull[0] = 1;
            _previousHull[1] = _nextHull[1] = 0;
        }
        return;
    }

    // Coincident inputs cannot supply a non-degenerate seed triangle.
    std::size_t distinctPoints = 0;
    for (std::size_t i = 0; i < n; ++i) {
        bool alreadySeen = false;
        for (std::size_t j = 0; j < i; ++j) {
            if (check_pts_equal(_coordinates[i], _coordinates[j])) {
                alreadySeen = true;
                break;
            }
        }
        if (!alreadySeen) {
            ++distinctPoints;
        }
    }
    if (distinctPoints < 3) {
        _previousHull.assign(n, INVALID_INDEX);
        _nextHull.assign(n, INVALID_INDEX);
        _triangleHull.assign(n, INVALID_INDEX);
        _initialHull = INVALID_INDEX;
        return;
    }

    double max_x = std::numeric_limits<double>::lowest();
    double max_y = std::numeric_limits<double>::lowest();
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    std::vector<std::size_t> ids;
    ids.reserve(n);

    for (std::size_t i = 0; i < n; i++) {
        const double x = _coordinates[i].getX();
        const double y = _coordinates[i].getY();

        if (x < min_x) min_x = x;
        if (y < min_y) min_y = y;
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;

        ids.push_back(i);
    }
    const Vector2 centroid((min_x + max_x) / 2, (min_y + max_y) / 2);
    double min_dist = std::numeric_limits<double>::max();

    std::size_t i0 = INVALID_INDEX;
    std::size_t i1 = INVALID_INDEX;
    std::size_t i2 = INVALID_INDEX;

    // pick a seed point close to the centroid
    for (std::size_t i = 0; i < n; i++) {
        const double d = dist(_coordinates[i], centroid);
        if (d < min_dist) {
            i0 = i;
            min_dist = d;
        }
    }


    min_dist = std::numeric_limits<double>::max();

    // find the point closest to the seed
    for (std::size_t i = 0; i < n; i++) {
        if (i == i0 || check_pts_equal(_coordinates[i], _coordinates[i0])) continue;
        const double d = dist(_coordinates[i], _coordinates[i0]);
        if (d < min_dist && d > 0.0) {
            i1 = i;
            min_dist = d;
        }
    }


    double min_radius = std::numeric_limits<double>::max();

    // find the third point which forms the smallest circumcircle with the first two
    for (std::size_t i = 0; i < n; i++) {
        if (i == i0 || i == i1 ||
            check_pts_equal(_coordinates[i], _coordinates[i0]) ||
            check_pts_equal(_coordinates[i], _coordinates[i1])) continue;

        const double r = circumradius(_coordinates[i0], _coordinates[i1], _coordinates[i]);

        if (r < min_radius) {
            i2 = i;
            min_radius = r;
        }
    }

    if (!(min_radius < std::numeric_limits<double>::max())) {
        throw std::runtime_error("not triangulation");
    }

    if (orient(_coordinates[i0], _coordinates[i1], _coordinates[i2])) {
        std::swap(i1, i2);
    }
    _center = circumcenter(_coordinates[i0], _coordinates[i1], _coordinates[i2]);

    // sort the points by distance from the seed triangle circumcenter
    std::sort(ids.begin(), ids.end(), compare{ _coordinates, _center });

    // initialize a hash table for storing edges of the advancing convex hull
    _hashSize = static_cast<std::size_t>(std::llround(std::ceil(std::sqrt(n))));
    _hash.resize(_hashSize);
    std::fill(_hash.begin(), _hash.end(), INVALID_INDEX);

    // initialize arrays for tracking the edges of the advancing convex hull
    _previousHull.resize(n);
    _nextHull.resize(n);
    _triangleHull.resize(n);

    _initialHull = i0;

    size_t hull_size = 3;

    _nextHull[i0] = _previousHull[i2] = i1;
    _nextHull[i1] = _previousHull[i0] = i2;
    _nextHull[i2] = _previousHull[i1] = i0;

    _triangleHull[i0] = 0;
    _triangleHull[i1] = 1;
    _triangleHull[i2] = 2;

    _hash[hashKey(_coordinates[i0])] = i0;
    _hash[hashKey(_coordinates[i1])] = i1;
    _hash[hashKey(_coordinates[i2])] = i2;

    std::size_t max_triangles = n < 3 ? 1 : 2 * n - 5;
    _triangles.reserve(max_triangles * 3);
    _halfEdges.reserve(max_triangles * 3);
    addTriangle(i0, i1, i2, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX);
    Vector2 previousPoint;
    bool hasPreviousPoint = false;
    for (std::size_t k = 0; k < n; k++) {
        const std::size_t i = ids[k];
        const Vector2& point = _coordinates[i];

        // Skip adjacent near-duplicates in distance-sorted order.
        if (hasPreviousPoint && check_pts_equal(point, previousPoint)) continue;
        previousPoint = point;
        hasPreviousPoint = true;

        // Skip seed triangle points.
        if (check_pts_equal(point, _coordinates[i0]) ||
            check_pts_equal(point, _coordinates[i1]) ||
            check_pts_equal(point, _coordinates[i2])) continue;

        // find a visible edge on the convex hull using edge hash
        std::size_t start = 0;

        size_t key = hashKey(point);
        for (size_t j = 0; j < _hashSize; j++) {
            start = _hash[fast_mod(key + j, _hashSize)];
            if (start != INVALID_INDEX && start != _nextHull[start]) break;
        }

        start = _previousHull[start];
        size_t e = start;
        size_t q;

        while (q = _nextHull[e], !orient(point, _coordinates[e], _coordinates[q])) { //TODO: does it works in a same way as in JS
            e = q;
            if (e == start) {
                e = INVALID_INDEX;
                break;
            }
        }

        if (e == INVALID_INDEX) continue; // likely a near-duplicate point; skip it

        // add the first triangle from the point
        std::size_t t = addTriangle(
            e,
            i,
            _nextHull[e],
            INVALID_INDEX,
            INVALID_INDEX,
            _triangleHull[e]);

        _triangleHull[i] = legalize(t + 2);
        _triangleHull[e] = t;
        hull_size++;

        // walk forward through the hull, adding more _triangles and flipping recursively
        std::size_t next = _nextHull[e];
        while (
            q = _nextHull[next],
            orient(point, _coordinates[next], _coordinates[q])) {
            t = addTriangle(next, i, q, _triangleHull[i], INVALID_INDEX, _triangleHull[next]);
            _triangleHull[i] = legalize(t + 2);
            _nextHull[next] = next; // mark as removed
            hull_size--;
            next = q;
        }

        // walk backward from the other side, adding more _triangles and flipping
        if (e == start) {
            while (
                q = _previousHull[e],
                orient(point, _coordinates[q], _coordinates[e])) {
                t = addTriangle(q, i, e, INVALID_INDEX, _triangleHull[e], _triangleHull[q]);
                legalize(t + 2);
                _triangleHull[q] = t;
                _nextHull[e] = e; // mark as removed
                hull_size--;
                e = q;
            }
        }

        // update the hull indices
        _previousHull[i] = e;
        _initialHull = e;
        _previousHull[next] = i;
        _nextHull[e] = i;
        _nextHull[i] = next;

        _hash[hashKey(point)] = i;
        _hash[hashKey(_coordinates[e])] = e;
    }
}

double Delaunator::getHullArea() const {
    if (_initialHull == INVALID_INDEX) return 0.0;

    std::vector<double> hull_area;
    size_t e = _initialHull;
    do {
        const Vector2& point = _coordinates[e];
        const Vector2& previousPoint = _coordinates[_previousHull[e]];
        hull_area.push_back(
            (point.getX() - previousPoint.getX()) *
            (point.getY() + previousPoint.getY()));
        e = _nextHull[e];
    } while (e != _initialHull);
    // The edge sum is twice the polygon area; divide by two for the area.
    return sum(hull_area) * 0.5;
}

std::size_t Delaunator::legalize(std::size_t a) {
    std::size_t i = 0;
    std::size_t ar = 0;
    _edgeStack.clear();

    // recursion eliminated with a fixed-size stack
    while (true) {
        const size_t b = _halfEdges[a];

        /* if the pair of _triangles doesn't satisfy the Delaunay condition
        * (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
        * then do the same check/flip recursively for the new pair of _triangles
        *
        *           pl                    pl
        *          /||\                  /  \
        *       al/ || \bl            al/    \a
        *        /  ||  \              /      \
        *       /  a||b  \    flip    /___ar___\
        *     p0\   ||   /p1   =>   p0\---bl---/p1
        *        \  ||  /              \      /
        *       ar\ || /br             b\    /br
        *          \||/                  \  /
        *           pr                    pr
        */
        const size_t a0 = 3 * (a / 3);
        ar = a0 + (a + 2) % 3;

        if (b == INVALID_INDEX) {
            if (i > 0) {
                i--;
                a = _edgeStack[i];
                continue;
            } else {
                //i = INVALID_INDEX;
                break;
            }
        }

        const size_t b0 = 3 * (b / 3);
        const size_t al = a0 + (a + 1) % 3;
        const size_t bl = b0 + (b + 2) % 3;

        const std::size_t p0 = _triangles[ar];
        const std::size_t pr = _triangles[a];
        const std::size_t pl = _triangles[al];
        const std::size_t p1 = _triangles[bl];

        const bool illegal = in_circle(
            _coordinates[p0], _coordinates[pr],
            _coordinates[pl], _coordinates[p1]);

        if (illegal) {
            _triangles[a] = p1;
            _triangles[b] = p0;

            auto hbl = _halfEdges[bl];

            // edge swapped on the other side of the hull (rare); fix the halfedge reference
            if (hbl == INVALID_INDEX) {
                std::size_t e = _initialHull;
                do {
                    if (_triangleHull[e] == bl) {
                        _triangleHull[e] = a;
                        break;
                    }
                    e = _nextHull[e];
                } while (e != _initialHull);
            }
            link(a, hbl);
            link(b, _halfEdges[ar]);
            link(ar, bl);
            std::size_t br = b0 + (b + 1) % 3;

            if (i < _edgeStack.size()) {
                _edgeStack[i] = br;
            } else {
                _edgeStack.push_back(br);
            }
            i++;

        } else {
            if (i > 0) {
                i--;
                a = _edgeStack[i];
                continue;
            } else {
                break;
            }
        }
    }
    return ar;
}

inline std::size_t Delaunator::hashKey(const Vector2& point) const {
    const Vector2 offset = point - _center;
    return fast_mod(
        static_cast<std::size_t>(std::llround(std::floor(pseudo_angle(offset) * static_cast<double>(_hashSize)))),
        _hashSize);
}

std::size_t Delaunator::addTriangle(
    std::size_t i0,
    std::size_t i1,
    std::size_t i2,
    std::size_t a,
    std::size_t b,
    std::size_t c) {
    std::size_t t = _triangles.size();
    _triangles.push_back(i0);
    _triangles.push_back(i1);
    _triangles.push_back(i2);
    link(t, a);
    link(t + 1, b);
    link(t + 2, c);
    return t;
}

void Delaunator::link(const std::size_t a, const std::size_t b) {
    std::size_t s = _halfEdges.size();
    if (a == s) {
        _halfEdges.push_back(b);
    } else if (a < s) {
        _halfEdges[a] = b;
    } else {
        throw std::runtime_error("Cannot link edge");
    }
    if (b != INVALID_INDEX) {
        std::size_t s2 = _halfEdges.size();
        if (b == s2) {
            _halfEdges.push_back(a);
        } else if (b < s2) {
            _halfEdges[b] = a;
        } else {
            throw std::runtime_error("Cannot link edge");
        }
    }
}



const std::vector<Vector2>& Delaunator::getCoordinates() const { return _coordinates; }
const std::vector<std::size_t>& Delaunator::getTriangles() const { return _triangles; }
const std::vector<std::size_t>& Delaunator::getHalfEdges() const { return _halfEdges; }
const std::vector<std::size_t>& Delaunator::getHullPrevious() const { return _previousHull; }
const std::vector<std::size_t>& Delaunator::getHullNext() const { return _nextHull; }
const std::vector<std::size_t>& Delaunator::getTriangleHull() const { return _triangleHull; }
std::size_t Delaunator::getHullStart() const { return _initialHull; }

} // namespace BurgerEngine2D
