#ifndef INTERFACE_IDRAWABLE_HPP
#define INTERFACE_IDRAWABLE_HPP

#include "Vector2.hpp"
#include "I_Positioned.hpp"

#include "raylib.h"

#include <optional>

//Forward declaration of namespace objects necessary for type declaration:
namespace BurgerEngine2D {
    class DrawStack2D;
}

namespace BurgerEngine2D {

    class I_Drawable : public virtual I_Positioned {
        private:
            int _zDepth;
            
        protected:
            I_Drawable() = default;
            BurgerEngine2D::Vector2 convertPosition(BurgerEngine2D::Vector2 position, 
                float scale, BurgerEngine2D::Vector2 offset) const;

            virtual void drawSpecific(BurgerEngine2D::Vector2 position, float scale) const = 0;
        public:
            virtual ~I_Drawable() = default;
            void Draw(BurgerEngine2D::Vector2 position, float scale, 
                    BurgerEngine2D::Vector2 offset) const;

            int getZDepth() const {
                return _zDepth;
            };
            void setZDepth(int val) {
                _zDepth = val;
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
