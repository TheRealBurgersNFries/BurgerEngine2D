#ifndef RAYLIBMOCK_HPP
#define RAYLIBMOCK_HPP

namespace BurgerEngine2D {
    void resetMockCalls();

    int getBeginDrawingCalls();
    int getEndDrawingCalls();
    int getInitWindowCalls();
    int getCloseWindowCalls();
    int getSetTargetCalls();
    int getWindowCloseCalls();
}

#endif // RAYLIBMOCK_HPP