
#include "PhysicsSystem2D.hpp"

#include <algorithm>
#include <execution>
#include <chrono>

namespace BurgerEngine2D {

    PhysicsSystem2D::PhysicsSystem2D()
        : _running(false) {
    }

    PhysicsSystem2D::~PhysicsSystem2D() {
        stop();
    }

    void PhysicsSystem2D::start() {
        if (_physicsThread.joinable()) {
            return;
        }

        _running.store(true);
        _physicsThread = std::thread(&PhysicsSystem2D::run, this);
    }

    void PhysicsSystem2D::stop() {
        _running.store(false);

        if (_physicsThread.joinable()) {
            _physicsThread.join();
        }
    }

    void PhysicsSystem2D::addCollidable(I_Collidable& collidable) {
        std::lock_guard<std::mutex> lock(_collidablesMutex);

        const auto found = std::find(
            _collidables.begin(),
            _collidables.end(),
            &collidable
        );

        if (found == _collidables.end()) {
            _collidables.push_back(&collidable);
        }
    }

    void PhysicsSystem2D::removeCollidable(I_Collidable& collidable) {
        std::lock_guard<std::mutex> lock(_collidablesMutex);

        _collidables.erase(
            std::remove(
                _collidables.begin(),
                _collidables.end(),
                &collidable
            ),
            _collidables.end()
        );
    }

    void PhysicsSystem2D::run() {
        using Clock = std::chrono::steady_clock;
        constexpr double timestep = 1.0 / 60.0;

        const auto interval = std::chrono::duration_cast<Clock::duration>(
            std::chrono::duration<double>(timestep)
        );

        auto nextTick = Clock::now();

        while (_running.load()) {
            nextTick += interval;

            physicsStep(static_cast<float>(timestep));

            std::this_thread::sleep_until(nextTick);

            if (Clock::now() > nextTick + interval) {
                nextTick = Clock::now();
            }
        }
    }

    void PhysicsSystem2D::physicsStep(float deltaTime) {
        const std::vector<I_Collidable*> collidables =
            getCollidableSnapshot();

        processMotion(collidables, deltaTime);
        
        // Motion processing, collision detection, and event dispatch follow.
    }

    std::vector<I_Collidable*> PhysicsSystem2D::getCollidableSnapshot() {
        std::lock_guard<std::mutex> lock(_collidablesMutex);

        return _collidables;
    }

    void PhysicsSystem2D::processMotion(const std::vector<I_Collidable*>& collidables,
        float deltaTime) {
        std::for_each(collidables.begin(), collidables.end(),
            [deltaTime](I_Collidable* collidable) {
                collidable->processMotion(deltaTime);
            }
        );
    }
}

