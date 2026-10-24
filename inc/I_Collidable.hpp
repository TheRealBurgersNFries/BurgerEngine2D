
#ifndef I_COLLIDABLE_HPP
#define I_COLLIDABLE_HPP

#include "Vector2.hpp"

namespace BurgerEngine2D {
    class I_Collidable {
        public:
            virtual ~I_Collidable() = default;

            virtual Vector2 getVelocity() const noexcept = 0;
            virtual void setVelocity(const Vector2& velocity) noexcept = 0;

            virtual Vector2 getAcceleration() const noexcept = 0;
            virtual void setAcceleration(
                const Vector2& acceleration
            ) noexcept = 0;

            virtual float getRotationalVelocity() const noexcept = 0;
            virtual void setRotationalVelocity(
                float velocity
            ) noexcept = 0;

            virtual float getRotationalAcceleration() const noexcept = 0;
            virtual void setRotationalAcceleration(
                float acceleration
            ) noexcept = 0;

            virtual float getMass() const noexcept = 0;
            virtual void setMass(float mass) noexcept = 0;

            virtual void onCollision(I_Collidable& other) = 0;
            
            virtual void processMotion(float deltaTime) = 0;
    };
}

#endif // I_COLLIDABLE_HPP
