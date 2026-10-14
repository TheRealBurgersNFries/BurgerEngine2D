#include "I_Drawable.hpp"

namespace BurgerEngine2D {

    int I_Drawable::getZDepth() {
        return _zDepth;
    }
    
    void I_Drawable::setZDepth(int val) {
        _zDepth = val;
    }

    bool I_Drawable::InStack() {
        return _inStack;
    }
}


