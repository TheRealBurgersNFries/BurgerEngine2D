#include "I_Drawable.hpp"

namespace BurgerEngine2D {
    
    BurgerEngine2D::Vector2 I_Drawable::convertPosition(BurgerEngine2D::Vector2 position,
             float scale, BurgerEngine2D::Vector2 offset) const{
        float x, y;
        x = (_worldPosition.getX() - position.getX()) * scale - offset.getX();
        y = (_worldPosition.getY() - position.getY()) * scale - offset.getY();
        //TODO track window size to get delta from window origin to centered screen position of camera
        return BurgerEngine2D::Vector2(x, y);
    }

    void I_Drawable::Draw(BurgerEngine2D::Vector2 position, float scale, BurgerEngine2D::Vector2 offset) const {
        position = convertPosition(position, scale, offset);
        drawSpecific(position, scale);

    }
}


