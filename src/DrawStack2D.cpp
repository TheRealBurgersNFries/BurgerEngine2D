#include "DrawStack2D.hpp"
#include "I_Drawable.hpp"
#include "KeyboardController.hpp"
#include "helper.hpp"

#include "RayLibWrapper.hpp"

#include <algorithm>
//#include <execution>
#include <unordered_set>

namespace BurgerEngine2D {
    DrawStack2D::DrawStack2D()
        : _defaultCamera(),
        _camera(&_defaultCamera),
        _keyboardController(nullptr),
        _maxZ(-1),
        _maxAddedZ(-1) {}

    void DrawStack2D::Draw(BurgerEngine2D::Vector2 offset) {
        if (_camera == nullptr) {
            throw std::runtime_error("Camera not set for DrawStack2D.");
        }
        if(_subList.size() > 0)
            removeSubs();
        if(_addList.size() > 0)
            addNew();
        beginDrawing();
        for (std::vector<I_Drawable*>::iterator drawable = _drawList.begin(); drawable !=  //
            _drawList.end(); drawable++) {
                (*drawable)->Draw(_camera->getPosition(), _camera->getScale(), offset);
        }
        endDrawing();

        if (_keyboardController != nullptr)
            _keyboardController->update();
    }

    void DrawStack2D::sortSubs() {
        std::sort(/*std::execution::par,*/ _subList.begin(), _subList.end(), lesserDepthComparer{});
    }

    void DrawStack2D::sortDraws() {
        std::sort(/*std::execution::par,*/ _drawList.begin(), _drawList.end(), lesserDepthComparer{});
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
                sub = _subList.erase(sub);
                draw = _drawList.erase(draw);
            }
        }
        _subList.clear();               
    }

    void DrawStack2D::addNew() {
        for (std::vector<I_Drawable*>::iterator drawable = _addList.begin(); drawable != //
        _addList.end(); drawable++) {
            _drawList.push_back(*drawable);
        }
        sortDraws();
        _addList.clear();
    }

    inline bool DrawStack2D::inDrawlist(I_Drawable* drawable) {
        return (std::find(_drawList.begin(), _drawList.end(), drawable) != _drawList.end());
    }

    inline bool DrawStack2D::inSublist(I_Drawable* drawable) {
        return (std::find(_subList.begin(), _subList.end(), drawable) != _subList.end());
    }

    inline bool DrawStack2D::inAddlist(I_Drawable* drawable) {
        return (std::find(_addList.begin(), _addList.end(), drawable) != _addList.end());
    }

    bool DrawStack2D::addToStack(I_Drawable* drawable) {
        if (!isInStack(drawable)){
            int currentZ = drawable->getZDepth();
            if (currentZ == 0)
                drawable->setZDepth(++_maxAddedZ);
            else if (currentZ > _maxAddedZ)
                _maxAddedZ = currentZ;
            _addList.push_back(drawable);
            return true;
        }
        return false;
    }

    bool DrawStack2D::isInStack(I_Drawable* drawable) {
        if (inAddlist(drawable))
            return true;
        if (inDrawlist(drawable) && !inSublist(drawable)) {
            return true;
        }
        return false;

    }

    void DrawStack2D::removeFromStack(I_Drawable* drawable) {
        auto pending = std::find(_addList.begin(), _addList.end(), drawable);

        if (pending != _addList.end()) {
            _addList.erase(pending);
            return;
        }
        if (inSublist(drawable) || !inDrawlist(drawable))
            return;
        _subList.push_back(drawable);
    }
    
    int DrawStack2D::getDrawCount() const{
        return (_drawList.size() + _addList.size() - _subList.size());
    }

    void DrawStack2D::setCamera(Camera2D* cam) {
        _camera = cam;
    }

    void DrawStack2D::setKeyboardController( KeyboardController* controller) {
        _keyboardController = controller;
    }
}