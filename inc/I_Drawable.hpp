#ifndef INTERFACE_IDRAWABLE_HPP
#define INTERFACE_IDRAWABLE_HPP

#include "raylib.h"
#include <vector>

//Forward declaration of namespace objects necessary for type declaration:
namespace BurgerEngine2D {
    class DrawStack2D;

}
namespace BurgerEngine2D {

    class I_Drawable {
        private:
            int _zDepth;
            bool _inStack = false;
            DrawStack2D* _stack;
        protected:

        public:
            virtual ~I_Drawable() = default;
            virtual void Draw() const = 0;

    };

}



#endif //INTERFACE_IDRAWABLE_HPP