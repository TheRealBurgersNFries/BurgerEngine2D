#ifndef DRAWSTACK_2D_HPP
#define DRAWSTACK_2D_HPP

#include <vector>

#include "raylib.h"

//Forward declaration of namespace objects necessary for type declaration:
namespace BurgerEngine2D {
    class I_Drawable;

}

namespace BurgerEngine2D {
    class DrawStack2D final {
        private:
            std::vector<I_Drawable*> _drawList;
        public:
            void Draw();
    };

}


#endif //DRAWSTACK_2D_HPP