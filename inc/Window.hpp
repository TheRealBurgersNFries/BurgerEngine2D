#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "DrawStack2D.hpp"
#include "Dispatcher.hpp"
#include "I_Subscriber.hpp"

#include "RayLibWrapper.hpp"

#include <string>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace BurgerEngine2D {
    class Window  : public I_Subscriber{
        private:
            std::atomic<bool>* _running;

            int _windowWidth;
            int _windowHeight;
            std::string _title;
            int _targetFramerate;
            BurgerEngine2D::DrawStack2D* _stack;
            
            std::vector<int>* _inputs;
            std::atomic<bool>* _inputsAvailable;

            bool _physicsComplete;
            std::mutex _physicsMutex;
            std::condition_variable _physicsCondition;
            
            std::jthread _renderThread;

            void onPhysicsComplete();

            void renderTask();

            bool getInputs();

        public:
            Window(std::atomic<bool>* running, int width, int height,
                std::string title, int targetFrameRate,
                BurgerEngine2D::DrawStack2D* stack, std::vector<int>* inputs,
                std::atomic<bool>* inputsAvailable);

            ~Window();

            Window(const Window&) = delete;
            Window& operator = (const Window&) = delete;

            void onEvent(std::shared_ptr<const I_Event> event) override;


        
    };
}

#endif // WINDOW_HPP
