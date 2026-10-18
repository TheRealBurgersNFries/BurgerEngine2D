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

    void setDrawDelay(int delay);
    void resetDrawDelay();
    
    // Configure mock behavior
    void setWindowShouldCloseResult(bool result);

    // Inspect initialization arguments
    int getLastWindowWidth();
    int getLastWindowHeight();
    // std::string getLastWindowTitle();
    int getLastTargetFPS();

}

#endif // RAYLIBMOCK_HPP