#ifndef I_POSITIONED_HPP
#define I_POSITIONED_HPP

#include "Vector2.hpp"

namespace BurgerEngine2D {

    // Shared world position for objects with multiple engine capabilities.
    class I_Positioned {
    public:
        virtual ~I_Positioned() = default;

        const Vector2& getPosition() const noexcept {
            return _position;
        }

        virtual void setPosition(const Vector2& position) noexcept {
            _position = position;
        }

    protected:
        I_Positioned() = default;

    private:
        Vector2 _position{};
    };

}

#endif // I_POSITIONED_HPP
