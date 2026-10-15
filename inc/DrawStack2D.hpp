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
            std::vector<I_Drawable*> _addList;
            std::vector<I_Drawable*> _subList;

            void sortSubs();
            void sortDraws();

            void removeSubs();
            void addNew();

            int _maxZ;

            int _maxAddedZ;
        public:
            void Draw();
            void AddToStack(I_Drawable *drawable);
            void removeFromStack(I_Drawable *drawable);
            int getDrawCount();
            int getMaxZDepth();

            

            DrawStack2D();
    };

}


#endif //DRAWSTACK_2D_HPP