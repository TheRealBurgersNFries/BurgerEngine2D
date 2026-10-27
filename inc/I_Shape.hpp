#ifndef INTERFACE_SHAPE_HPP
#define INTERFACE_SHAPE_HPP

#include <vector>

#include "Vector2.hpp"

namespace BurgerEngine2D {

    class I_Shape {
        private:
            std::vector<Vector2> _points;
            Vector2 _center;
            float _radius;

        protected:
            virtual void findCenter();
            virtual void findRadius();
            
            public:
            explicit I_Shape(const std::vector<Vector2>& points);
            virtual ~I_Shape() = default;
            
            void setPosition(const Vector2& position);

            const std::vector<Vector2>& getPoints() const;
            const Vector2& getCenter() const;
            float getRadius() const;
    };

}

#endif // INTERFACE_SHAPE_HPP