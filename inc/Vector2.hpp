#ifndef BE2D_VECTOR2_HPP
#define BE2D_VECTOR2_HPP

namespace BurgerEngine2D {

    class Vector2 final {
        private:
            float _x;
            float _y;
      
        public:
            constexpr Vector2()
                : _x(0.0f), _y(0.0f) {
            }
        
            constexpr Vector2(float x, float y) 
                : _x(x), _y(y) {
            }

            constexpr float getX() const {
                return _x;
            }

            constexpr float getY() const {
                return _y;
            }
            
            constexpr Vector2 operator+(const Vector2& other) const {
                return {_x + other._x, _y + other._y};
            }

            constexpr Vector2 operator-(const Vector2& other) const {
                return {_x - other._x, _y - other._y};
            }

            constexpr Vector2 operator*(float scalar) const {
                return {_x * scalar, _y * scalar};
            }

            constexpr Vector2 operator/(float scalar) const {
                return {_x / scalar, _y / scalar};
            }

            constexpr bool operator==(const Vector2& other) const
            {
                return _x == other._x && _y == other._y;
            }

    };
}

#endif // BE2D_VECTOR2_HPP