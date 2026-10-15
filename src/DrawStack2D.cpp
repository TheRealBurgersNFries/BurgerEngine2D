#include "DrawStack2D.hpp"
#include "I_Drawable.hpp"
#include "helper.hpp"

#include "RayLibWrapper.hpp"

#include <algorithm>
#include <unordered_set>

namespace BurgerEngine2D {
    
    DrawStack2D::DrawStack2D() {
        _maxZ = -1;
        _maxAddedZ = -1;
    }

    void DrawStack2D::Draw() {
        if(_subList.size() > 0)
            removeSubs();
        if(_addList.size() > 0)
            addNew();
        BurgerEngine2D::beginDrawing();
        for (std::vector<I_Drawable*>::iterator drawable = _drawList.begin(); drawable !=  //
            _drawList.end(); drawable++) {
                (*drawable)->Draw();
        }
        BurgerEngine2D::endDrawing();
    }

    void DrawStack2D::AddToStack(I_Drawable* drawable) {
        int currentZ = drawable->getZDepth();
        if (currentZ == 0)
            drawable->setZDepth(++_maxAddedZ);
        else if (currentZ > _maxAddedZ)
            _maxAddedZ = currentZ;
        drawable->addToStack(this);
        _addList.push_back(drawable);
    }

    int DrawStack2D::getDrawCount() {
        return (_drawList.size() + _addList.size() - _subList.size());
    }

    void DrawStack2D::sortSubs() {
        std::sort( _subList.begin(), _subList.end(), lesserDepthComparer{});
    }

    void DrawStack2D::removeSubs() {
        sortSubs();

        auto draw = _drawList.begin();
        auto sub  = _subList.begin();

        while (draw != _drawList.end() && sub != _subList.end()) {
            if (lesserDepthComparer{}(*draw, *sub)) {
                ++draw;
            } else if (lesserDepthComparer{}(*sub, *draw)) {
                ++sub;
            } else {
                // Same pointer: remove from both vectors.
                (*sub)->removeFromStack();

                draw = _drawList.erase(draw);
                ++sub;
            }
        }
        _subList.clear();               
    }

    void DrawStack2D::removeFromStack(I_Drawable* drawable) {
        _subList.push_back(drawable);
    }

    void DrawStack2D::addNew() {
        for (std::vector<I_Drawable*>::iterator drawable = _addList.begin(); drawable != //
        _addList.end(); drawable++) {
            _drawList.push_back(*drawable);
            (*drawable)->addToStack(this);
        }
        _addList.clear();
    }
    
}