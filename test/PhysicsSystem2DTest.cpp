
#include "CppUTest/TestHarness.h"

#include <chrono>
#include <thread>

#include "PhysicsSystem2D.hpp"

TEST_GROUP(PhysicsSystem2DTest) {
    BurgerEngine2D::PhysicsSystem2D* physicsSystem;

    void setup() {
        physicsSystem = new BurgerEngine2D::PhysicsSystem2D();
    }

    void teardown() {
        delete physicsSystem;
        physicsSystem = nullptr;
    }
};

TEST(PhysicsSystem2DTest, ConstructSystem) {
    // Construction should succeed without starting the physics thread.
}

TEST(PhysicsSystem2DTest, StopBeforeStart) {
    physicsSystem->stop();

    // Stopping a system that was never started should be safe.
}

TEST(PhysicsSystem2DTest, StartAndStop) {
    physicsSystem->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    physicsSystem->stop();
}

TEST(PhysicsSystem2DTest, StartingTwiceDoesNotCreateAnotherThread) {
    physicsSystem->start();
    physicsSystem->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    physicsSystem->stop();
}

TEST(PhysicsSystem2DTest, StoppingTwiceIsSafe) {
    physicsSystem->start();

    physicsSystem->stop();
    physicsSystem->stop();
}

TEST(PhysicsSystem2DTest, CanRestartAfterStopping) {
    physicsSystem->start();
    physicsSystem->stop();

    physicsSystem->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    physicsSystem->stop();
}

TEST(PhysicsSystem2DTest, DestructorStopsRunningThread) {
    {
        BurgerEngine2D::PhysicsSystem2D system;

        system.start();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // Leaving scope should stop and join the physics thread.
}
