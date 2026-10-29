
#include "I_Shape.hpp"

#include <cmath>

namespace BurgerEngine2D {

    I_Shape::I_Shape(const std::vector<Vector2>& points)
        : _points(points), _center(), _radius(0.0) {
        findCenter();
        findRadius();
    }

    void I_Shape::findCenter() {
        if (_points.empty()) {
            _center = Vector2();
            return;
        }

        double x = 0.0;
        double y = 0.0;

        for (const Vector2& point : _points) {
            x += point.getX();
            y += point.getY();
        }

        const double count = static_cast<double>(_points.size());

        _center = Vector2(x / count, y / count);
    }

    void I_Shape::findRadius() {
        _radius = 0.0;

        for (const Vector2& point : _points) {
            const double dx = point.getX() - _center.getX();
            const double dy = point.getY() - _center.getY();
            const double distance = std::sqrt(dx * dx + dy * dy);

            if (distance > _radius) {
                _radius = distance;
            }
        }
    }

    void I_Shape::setPosition(const Vector2& position) {
        _center = position;
    }

    const std::vector<Vector2>& I_Shape::getPoints() const {
        return _points;
    }

    const Vector2& I_Shape::getCenter() const {
        return _center;
    }

    double I_Shape::getRadius() const {
        return _radius;
    }

}
