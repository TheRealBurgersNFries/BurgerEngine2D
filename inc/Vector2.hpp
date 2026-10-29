#ifndef BE2D_VECTOR2_HPP
#define BE2D_VECTOR2_HPP

namespace BurgerEngine2D {

    class Vector2 final {
        private:
            double _x;
            double _y;
      
        public:
            constexpr Vector2()
                : _x(0.0), _y(0.0) {
            }
        
            constexpr Vector2(double x, double y)
                : _x(x), _y(y) {
            }

            constexpr double getX() const {
                return _x;
            }

            constexpr double getY() const {
                return _y;
            }
            
            constexpr Vector2 operator+(const Vector2& other) const {
                return {_x + other._x, _y + other._y};
            }

            constexpr Vector2 operator-(const Vector2& other) const {
                return {_x - other._x, _y - other._y};
            }

            constexpr Vector2 operator*(double scalar) const {
                return {_x * scalar, _y * scalar};
            }

            constexpr Vector2 operator/(double scalar) const {
                return {_x / scalar, _y / scalar};
            }

            /** Returns the dot product with another vector. */
            constexpr double dot(const Vector2& other) const {
                return _x * other._x + _y * other._y;
            }

            /** Returns the signed 2D cross product with another vector. */
            constexpr double cross(const Vector2& other) const {
                return _x * other._y - _y * other._x;
            }

            /** Returns the squared distance from the origin. */
            constexpr double lengthSquared() const {
                return dot(*this);
            }

            /** Returns the squared distance to another point. */
            constexpr double distanceSquared(const Vector2& other) const {
                return (*this - other).lengthSquared();
            }

            /** Returns this vector's projection onto another vector. */
            constexpr Vector2 project(const Vector2& onto) const {
                const double targetLengthSquared = onto.lengthSquared();
                if (targetLengthSquared == 0.0) {
                    return Vector2();
                }

                return onto * (dot(onto) / targetLengthSquared);
            }

            constexpr bool operator==(const Vector2& other) const
            {
                return _x == other._x && _y == other._y;
            }

    };
}

#endif // BE2D_VECTOR2_HPP
