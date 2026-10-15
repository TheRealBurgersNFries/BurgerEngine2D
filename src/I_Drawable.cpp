#include "I_Drawable.hpp"

namespace BurgerEngine2D {

    int I_Drawable::getZDepth() const {
        return _zDepth;
    }
    
    void I_Drawable::setZDepth(int val) {
        _zDepth = val;
    }

    bool I_Drawable::InStack() const {
        return (_stack.has_value());
    }

    void I_Drawable::addToStack(DrawStack2D* stack) {
        _stack = &*stack;

    }
    void I_Drawable::removeFromStack() {
        _stack.reset();
    }
}


