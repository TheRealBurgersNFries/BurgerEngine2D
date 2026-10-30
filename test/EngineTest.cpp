#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "Engine.hpp"
#include "I_Collidable.hpp"
#include "I_Drawable.hpp"
#include "event/Dispatcher.hpp"
#include "event/EventType.hpp"
#include "event/I_Event.hpp"
#include "event/I_Subscriber.hpp"

#include "CppUTest/TestHarness.h"

namespace {

    class FacadeDrawable : public BurgerEngine2D::I_Drawable {
    protected:
        void drawSpecific(BurgerEngine2D::Vector2, float) const override {}
    };

    class FacadeCollidable : public BurgerEngine2D::I_Collidable {
        public:
            FacadeCollidable()
                : BurgerEngine2D::I_Collidable({
                    BurgerEngine2D::Vector2(-1.0, 0.0),
                    BurgerEngine2D::Vector2(1.0, 0.0),
                    BurgerEngine2D::Vector2(0.0, -1.0),
                    BurgerEngine2D::Vector2(0.0, 1.0)
                }),
                _velocity(0.0, 0.0),
                _acceleration(0.0, 0.0),
                _motionCount(0) {}

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
                return 0.0f;
            }

            void setRotationalVelocity(float) noexcept override {}

            float getRotationalAcceleration() const noexcept override {
                return 0.0f;
            }

            void setRotationalAcceleration(float) noexcept override {}

            float getMass() const noexcept override {
                return 1.0f;
            }

            void setMass(float) noexcept override {}

            void onCollision(BurgerEngine2D::I_Collidable&) override {}

            void processMotion(float deltaTime) override {
                BurgerEngine2D::I_Collidable::processMotion(deltaTime);
                _motionCount.fetch_add(1);
            }

            int getMotionCount() const noexcept {
                return _motionCount.load();
            }

        private:
            BurgerEngine2D::Vector2 _velocity;
            BurgerEngine2D::Vector2 _acceleration;
            std::atomic<int> _motionCount;
    };

    class FacadeEvent : public BurgerEngine2D::I_Event {
        public:
            BurgerEngine2D::EventType getType() const noexcept override {
                return BurgerEngine2D::GetEventType<FacadeEvent>();
            }
    };

    class FacadeSubscriber : public BurgerEngine2D::I_Subscriber {
        public:
            void onEvent(
                std::shared_ptr<const BurgerEngine2D::I_Event> event
            ) override {
                if (event && event->getType() ==
                        BurgerEngine2D::GetEventType<FacadeEvent>()) {
                    ++_eventCount;
                }
            }

            int getEventCount() const noexcept {
                return _eventCount;
            }

        private:
            int _eventCount = 0;
    };

}

TEST_GROUP(EngineFacadeTestGroup) {
    void setup() {
        BurgerEngine2D::Dispatcher::dispatch();
        BurgerEngine2D::Dispatcher::clearAllSubscribers();
    }

    void teardown() {
        BurgerEngine2D::Dispatcher::dispatch();
        BurgerEngine2D::Dispatcher::clearAllSubscribers();
    }
};

TEST(EngineFacadeTestGroup, AddsAndRemovesDrawableThroughFacade) {
    BurgerEngine2D::Engine engine;
    FacadeDrawable drawable;

    CHECK_TRUE(engine.addDrawable(drawable));
    CHECK_FALSE(engine.addDrawable(drawable));
    CHECK_TRUE(engine.getDrawStack().isInStack(&drawable));

    engine.removeDrawable(drawable);

    CHECK_FALSE(engine.getDrawStack().isInStack(&drawable));
}

TEST(EngineFacadeTestGroup, ExposesDirectControlOfOwnedSystems) {
    BurgerEngine2D::Engine engine;
    FacadeDrawable drawable;

    CHECK_TRUE(engine.getDrawStack().addToStack(&drawable));
    CHECK_TRUE(engine.getDrawStack().isInStack(&drawable));
    CHECK_FALSE(engine.isRunning());

    engine.getPhysicsSystem().start();
    engine.getPhysicsSystem().stop();
}

TEST(EngineFacadeTestGroup, RegistersCollidableWithOwnedPhysicsSystem) {
    BurgerEngine2D::Engine engine;
    FacadeCollidable collidable;

    engine.addCollidable(collidable);
    engine.getPhysicsSystem().start();

    const auto deadline = std::chrono::steady_clock::now() +
        std::chrono::seconds(1);
    while (collidable.getMotionCount() == 0 &&
           std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    engine.getPhysicsSystem().stop();

    CHECK_TRUE(collidable.getMotionCount() > 0);
}

TEST(EngineFacadeTestGroup, QueuesEventsAndDeliversThemToTypedSubscribers) {
    BurgerEngine2D::Engine engine;
    FacadeSubscriber subscriber;

    engine.addEventSubscriber<FacadeEvent>(subscriber);
    engine.queueEvent(std::make_shared<FacadeEvent>());

    CHECK_EQUAL(0, subscriber.getEventCount());

    engine.getDispatcher().dispatch();

    CHECK_EQUAL(1, subscriber.getEventCount());
}


