
#ifndef PHYSICS_SYSTEM2D_HPP
#define PHYSICS_SYSTEM2D_HPP

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "I_Collidable.hpp"

    namespace BurgerEngine2D {

    class PhysicsSystem2D {
    public:
        PhysicsSystem2D();
        ~PhysicsSystem2D();

        PhysicsSystem2D(const PhysicsSystem2D&) = delete;
        PhysicsSystem2D& operator=(const PhysicsSystem2D&) = delete;

        void start();
        void stop();

        void addCollidable(I_Collidable& collidable);
        void removeCollidable(I_Collidable& collidable);

    private:
        void run();
        void physicsStep(float deltaTime);

        std::vector<I_Collidable*> getCollidableSnapshot();
        void processMotion(const std::vector<I_Collidable*>& collidables, 
            float deltaTime);

        std::vector<I_Collidable*> _collidables;
        std::mutex _collidablesMutex;

        std::thread _physicsThread;
        std::atomic<bool> _running;
    };

}

#endif
