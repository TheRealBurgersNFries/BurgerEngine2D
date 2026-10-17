#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include "DrawStack2D.hpp"

#include "Vector2.hpp"

#include <memory>

namespace BurgerEngine2D {
    

    class Camera2D {
        private:
            BurgerEngine2D::Vector2 _position;
            float _scale;
        public:
            constexpr Camera2D() 
                : _position(BurgerEngine2D::Vector2()), _scale(1) {
            }

            constexpr Camera2D(BurgerEngine2D::Vector2 pos, float scale) 
                :_position(pos), _scale(scale) {
            }

            inline BurgerEngine2D::Vector2 getPosition() const {
                return _position;
            }

            inline void setPostion(BurgerEngine2D::Vector2 position) {
                _position = position;
            }

            inline float getScale() const {
                return _scale;
            }

            inline void setScale(float scale) {
                _scale = scale;
            }
        
    };

}

#endif // CAMERA2D_HPP