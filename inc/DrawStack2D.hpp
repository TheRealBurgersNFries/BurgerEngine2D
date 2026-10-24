#ifndef DRAWSTACK_2D_HPP
#define DRAWSTACK_2D_HPP

#include <vector>
#include "raylib.h"

#include "Camera2D.hpp"

//Forward declaration of namespace objects necessary for type declaration:
namespace BurgerEngine2D {
    class I_Drawable;
    class KeyboardController;
    class Vector2;

}

namespace BurgerEngine2D {
    class DrawStack2D final {
        private:
            std::vector<I_Drawable*> _drawList;
            std::vector<I_Drawable*> _addList;
            std::vector<I_Drawable*> _subList;

            Camera2D _defaultCamera;
            Camera2D* _camera;
            KeyboardController* _keyboardController;

            void sortSubs();
            void sortDraws();

            void removeSubs();
            void addNew();

            bool inDrawlist(I_Drawable* drawable);
            bool inSublist(I_Drawable* drawable);
            bool inAddlist(I_Drawable* drawable);

            int _maxZ;

            int _maxAddedZ;
        public:
            void Draw(BurgerEngine2D::Vector2 offset);

            bool addToStack(I_Drawable* drawable);
            bool isInStack(I_Drawable* drawable);
            void removeFromStack(I_Drawable* drawable);

            void setCamera(Camera2D* cam);
            Camera2D* getCamera() const {
                return _camera;
            };

            void setKeyboardController(KeyboardController* controller);

            int getDrawCount() const;
            int getMaxZDepth() const {
                return _maxZ;
            };


            DrawStack2D();

            DrawStack2D(const DrawStack2D&) = delete;
            DrawStack2D& operator = (const DrawStack2D&) = delete;

    };

}


#endif //DRAWSTACK_2D_HPP