#ifndef INTERFACE_IDRAWABLE_HPP
#define INTERFACE_IDRAWABLE_HPP

#include "Vector2.hpp"

#include "raylib.h"

#include <optional>

//Forward declaration of namespace objects necessary for type declaration:
namespace BurgerEngine2D {
    class DrawStack2D;
}

namespace BurgerEngine2D {

    class I_Drawable {
        private:
            int _zDepth;
            
        protected:
            I_Drawable() = default;
            BurgerEngine2D::Vector2 _worldPosition;
            BurgerEngine2D::Vector2 convertPosition(BurgerEngine2D::Vector2 position, float scale) const;

            virtual void drawSpecific(BurgerEngine2D::Vector2 position, float scale) const = 0;
        public:
            virtual ~I_Drawable() = default;
            void Draw(BurgerEngine2D::Vector2 position, float scale) const;

            int getZDepth() const {
                return _zDepth;
            };
            BurgerEngine2D::Vector2 getPosition() const {
                return _worldPosition;
            };

            
            void setZDepth(int val) {
                _zDepth = val;
            }
            void setPosition(BurgerEngine2D::Vector2 position) {
                _worldPosition = position;
            }

            
            bool operator>(const I_Drawable &other) const {
                    return (getZDepth() > other.getZDepth());
                };
            bool operator<(const I_Drawable &other) const {
                    return (getZDepth() < other.getZDepth());
                };

    };

}

#endif //INTERFACE_IDRAWABLE_HPP