#include "I_Collidable.hpp"


namespace BurgerEngine2D {

        void I_Collidable::processMotion(float dt) {
        const Vector2 velocity = getVelocity() + getAcceleration() * dt;
        setVelocity(velocity);
        setPosition(getPosition() + velocity * dt);
    }
}
