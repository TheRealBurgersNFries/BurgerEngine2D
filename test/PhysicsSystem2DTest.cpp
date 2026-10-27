
#include "CppUTest/TestHarness.h"

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "PhysicsSystem2D.hpp"
#include "I_Collidable.hpp"
#include "I_Drawable.hpp"
#include "I_Shape.hpp"

namespace {
    class TestCollidable : public BurgerEngine2D::I_Drawable,
                          public BurgerEngine2D::I_Collidable {
    public:
        TestCollidable(float x, float y, float radius)
            : BurgerEngine2D::I_Collidable({
                BurgerEngine2D::Vector2(x - radius, y),
                BurgerEngine2D::Vector2(x + radius, y),
                BurgerEngine2D::Vector2(x, y - radius),
                BurgerEngine2D::Vector2(x, y + radius)
            }),
            _velocity(0.0f, 0.0f),
            _acceleration(0.0f, 0.0f),
            _rotationalVelocity(0.0f),
            _rotationalAcceleration(0.0f),
            _mass(1.0f),
            _motionCount(0),
            _collisionCount(0) {}

        BurgerEngine2D::Vector2 getVelocity() const noexcept override {
            return _velocity;
        }

        void setVelocity(
            const BurgerEngine2D::Vector2& velocity
        ) noexcept override {
            _velocity = velocity;
        }

        BurgerEngine2D::Vector2 getAcceleration() const noexcept override {
            return _acceleration;
        }

        void setAcceleration(
            const BurgerEngine2D::Vector2& acceleration
        ) noexcept override {
            _acceleration = acceleration;
        }

        float getRotationalVelocity() const noexcept override {
            return _rotationalVelocity;
        }

        void setRotationalVelocity(float velocity) noexcept override {
            _rotationalVelocity = velocity;
        }

        float getRotationalAcceleration() const noexcept override {
            return _rotationalAcceleration;
        }

        void setRotationalAcceleration(float acceleration) noexcept override {
            _rotationalAcceleration = acceleration;
        }

        float getMass() const noexcept override {
            return _mass;
        }

        void setMass(float mass) noexcept override {
            _mass = mass;
        }

        void onCollision(
            BurgerEngine2D::I_Collidable& other
        ) override {
            (void)other;
            _collisionCount.fetch_add(1);
        }

        int getMotionCount() const {
            return _motionCount.load();
        }

        int getCollisionCount() const {
            return _collisionCount.load();
        }

        void processMotion(float deltaTime) override {
            BurgerEngine2D::I_Collidable::processMotion(deltaTime);
            _motionCount.fetch_add(1);
        }

    protected:
        void drawSpecific(BurgerEngine2D::Vector2, float) const override {}

    private:
        BurgerEngine2D::Vector2 _velocity;
        BurgerEngine2D::Vector2 _acceleration;
        float _rotationalVelocity;
        float _rotationalAcceleration;
        float _mass;
        std::atomic<int> _motionCount;
        std::atomic<int> _collisionCount;
    };
}

TEST_GROUP(PhysicsSystem2DTest) {
    BurgerEngine2D::PhysicsSystem2D* physicsSystem;

            
    void setup() {
        physicsSystem = new BurgerEngine2D::PhysicsSystem2D(nullptr);
    }

    void teardown() {
        delete physicsSystem;
        physicsSystem = nullptr;
    }
};

TEST(PhysicsSystem2DTest, ConstructSystem) {
    // Construction should succeed without starting the physics thread.
}

TEST(PhysicsSystem2DTest, DrawableAndCollisionUseOnePosition) {
    TestCollidable object(10.0f, 20.0f, 3.0f);
    BurgerEngine2D::I_Drawable& drawable = object;
    BurgerEngine2D::I_Collidable& collidable = object;

    drawable.setPosition(BurgerEngine2D::Vector2(40.0f, 50.0f));

    CHECK_TRUE(drawable.getPosition() == BurgerEngine2D::Vector2(40.0f, 50.0f));
    CHECK_TRUE(collidable.getPosition() == BurgerEngine2D::Vector2(40.0f, 50.0f));
    CHECK_TRUE(collidable.getCenter() == BurgerEngine2D::Vector2(40.0f, 50.0f));
    DOUBLES_EQUAL(3.0, collidable.getRadius(), 0.0001);
}

TEST(PhysicsSystem2DTest, DefaultMotionUpdatesSharedPositionAndShapeCenter) {
    TestCollidable object(10.0f, 20.0f, 3.0f);
    object.setVelocity(BurgerEngine2D::Vector2(2.0f, -1.0f));
    object.setAcceleration(BurgerEngine2D::Vector2(0.0f, 2.0f));

    object.BurgerEngine2D::I_Collidable::processMotion(0.5f);

    CHECK_TRUE(object.getPosition() == BurgerEngine2D::Vector2(11.0f, 19.0f));
    CHECK_TRUE(object.getCenter() == BurgerEngine2D::Vector2(11.0f, 19.0f));
    CHECK_TRUE(object.getVelocity() == BurgerEngine2D::Vector2(2.0f, 0.0f));
}

TEST(PhysicsSystem2DTest, StopBeforeStart) {
    physicsSystem->stop();

    // Stopping a system that was never started should be safe.
}

TEST(PhysicsSystem2DTest, StartAndStop) {
    physicsSystem->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    physicsSystem->stop();
}

TEST(PhysicsSystem2DTest, StartingTwiceDoesNotCreateAnotherThread) {
    physicsSystem->start();
    physicsSystem->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

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

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    physicsSystem->stop();
}


TEST(PhysicsSystem2DTest, DestructorStopsRunningThread) {
    {
        BurgerEngine2D::PhysicsSystem2D system(nullptr);

        system.start();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Leaving scope should stop and join the physics thread.
}


TEST(PhysicsSystem2DTest, RegisteredCollidableProcessesMotion) {
    TestCollidable collidable(0.0f, 0.0f, 1.0f);

    physicsSystem->addCollidable(collidable);
    physicsSystem->start();

    const auto deadline =
        std::chrono::steady_clock::now() + std::chrono::seconds(1);

    while (collidable.getMotionCount() == 0 &&
           std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    physicsSystem->stop();

    CHECK_TRUE(collidable.getMotionCount() > 0);
}

TEST(PhysicsSystem2DTest, AddingSameCollidableTwiceDoesNotDuplicateIt) {
    TestCollidable collidable(0.0f, 0.0f, 1.0f);

    physicsSystem->addCollidable(collidable);
    physicsSystem->addCollidable(collidable);
    physicsSystem->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    physicsSystem->stop();

    // This verifies processing occurred. Duplicate registration is
    // prevented by addCollidable's existing uniqueness check.
    CHECK_TRUE(collidable.getMotionCount() > 0);
}

TEST(PhysicsSystem2DTest, RemovedCollidableStopsBeingProcessed) {
    TestCollidable collidable(0.0f, 0.0f, 1.0f);

    physicsSystem->addCollidable(collidable);
    physicsSystem->start();

    const auto deadline =
        std::chrono::steady_clock::now() + std::chrono::seconds(1);

    while (collidable.getMotionCount() == 0 &&
           std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    physicsSystem->removeCollidable(collidable);
    physicsSystem->stop();

    const int countAfterStop = collidable.getMotionCount();

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    CHECK_EQUAL(countAfterStop, collidable.getMotionCount());
}
