
#include "PhysicsSystem2D.hpp"

#include <algorithm>
// #include <execution>
#include <chrono>
#include <memory>

#include "Dispatcher.hpp"
#include "CollisionEvent.hpp"

namespace BurgerEngine2D {

    PhysicsSystem2D::PhysicsSystem2D(BurgerEngine2D::Dispatcher* dispatcher)
        : _running(false), _dispatcher(dispatcher) {
    }

    PhysicsSystem2D::~PhysicsSystem2D() {
        stop();
    }

    void PhysicsSystem2D::start() {
        if (_physicsThread.joinable()) {
            return;
        }

        _running.store(true);
        _physicsThread = std::jthread(&PhysicsSystem2D::run, this);
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
        
        detectCollisions(collidables);
        // Motion processing, collision detection, and event dispatch follow.
    }

    std::vector<I_Collidable*> PhysicsSystem2D::getCollidableSnapshot() {
        std::lock_guard<std::mutex> lock(_collidablesMutex);

        return _collidables;
    }

    void PhysicsSystem2D::detectCollisions(
        const std::vector<I_Collidable*>& collidables) {
        for (auto current = collidables.begin(); current != collidables.end();
                ++current) {
            I_Collidable* first = *current;

            if (first == nullptr) {
                continue;
            }

            std::for_each(
                current + 1,
                collidables.end(),
                [this, first](I_Collidable* other) {
                    if (other == nullptr) {
                        return;
                    }

                    if (isCircleColliding(first, other)) {
                        if (_dispatcher != nullptr) {
                            _dispatcher->queueEvent(
                                std::make_shared<CollisionEvent>(
                                    first, other)
                            );
                        }
                    }
                }
            );
        }
    }


    bool PhysicsSystem2D::isCircleColliding (const BurgerEngine2D::I_Collidable* a, 
        const BurgerEngine2D::I_Collidable* b) {
            const double dx = a->getCenter().getX() - b->getCenter().getX();
            const double dy = a->getCenter().getY() - b->getCenter().getY();

            const double radiusSum = a->getRadius() + b->getRadius();

            return (dx * dx + dy * dy) < (radiusSum * radiusSum);
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

