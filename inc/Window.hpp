#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "DrawStack2D.hpp"

#include "RayLibWrapper.hpp"

#include <string>
#include <atomic>

namespace BurgerEngine2D {
    class Window {
        private:
            BurgerEngine2D::DrawStack2D* _stack;
            int _windowWidth;
            int _windowHeight;
            int _targetFramerate;
            std::string _title;
            std::atomic<bool>* _running;
            std::atomic<bool>* _inputsAvailable;

            std::vector<int>* _inputs;

            void renderTask();

            void getInputs();

        public:
            Window(std::atomic<bool>* running, int width, int height,
                std::string title, int targetFrameRate,
                BurgerEngine2D::DrawStack2D* stack, std::vector<int>* inputs,
                std::atomic<bool>* inputsAvailable);

            Window(const Window&) = delete;
            Window& operator = (const Window&) = delete;


        
    };
}

#endif // WINDOW_HPP