#ifndef INTERFACE_IDRAWABLE_HPP
#define INTERFACE_IDRAWABLE_HPP

#include "raylib.h"
#include <vector>
#include <optional>

//Forward declaration of namespace objects necessary for type declaration:
namespace BurgerEngine2D {
    class DrawStack2D;

}
namespace BurgerEngine2D {

    class I_Drawable {
        private:
            int _zDepth;
            std::optional<DrawStack2D*> _stack;
            
        protected:
            I_Drawable() = default;

        public:
            virtual ~I_Drawable() = default;
            virtual void Draw() const = 0;
            int getZDepth() const;
            bool InStack() const;
            
            void setZDepth(int val);
            void addToStack(DrawStack2D* stack);
            void removeFromStack();

            bool operator>(const I_Drawable &other) const {
                    return (getZDepth() > other.getZDepth());
                };
            bool operator<(const I_Drawable &other) const {
                    return (getZDepth() < other.getZDepth());
                };

    };

}

#endif //INTERFACE_IDRAWABLE_HPP