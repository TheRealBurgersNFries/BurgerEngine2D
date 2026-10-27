#include "I_Drawable.hpp"

namespace BurgerEngine2D {
    
    BurgerEngine2D::Vector2 I_Drawable::convertPosition(BurgerEngine2D::Vector2 position,
             float scale, BurgerEngine2D::Vector2 offset) const{
        float x, y;
        x = (getPosition().getX() - position.getX()) * scale - offset.getX();
        y = (getPosition().getY() - position.getY()) * scale - offset.getY();
        return BurgerEngine2D::Vector2(x, y);
    }

    void I_Drawable::Draw(BurgerEngine2D::Vector2 position, float scale, BurgerEngine2D::Vector2 offset) const {
        position = convertPosition(position, scale, offset);
        drawSpecific(position, scale);

    }
}


