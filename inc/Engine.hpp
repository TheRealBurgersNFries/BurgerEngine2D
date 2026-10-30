#ifndef BE2D_ENGINE_HPP
#define BE2D_ENGINE_HPP

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "DrawStack2D.hpp"
#include "I_Collidable.hpp"
#include "I_Drawable.hpp"
#include "PhysicsSystem2D.hpp"
#include "event/Dispatcher.hpp"

namespace BurgerEngine2D {

    class Window;

    /** Settings used when the engine creates its window. */
    struct EngineConfig {
        int windowWidth = 800;
        int windowHeight = 600;
        std::string windowTitle = "BurgerEngine2D";
        int targetFrameRate = 60;
    };

    /**
     * Owns the engine's window, draw stack, and physics system.
     *
     * Add application objects through this class, then call run() on the
     * application's main thread. Registered objects must outlive the engine
     * registration, or be removed before they are destroyed.
     */
    class Engine final {
    public:
        explicit Engine(const EngineConfig& config = EngineConfig());
        ~Engine();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        /** Starts the window and physics threads and runs until stopped. */
        void run();

        /** Requests shutdown; run() performs final window cleanup. */
        void stop();

        bool isRunning() const noexcept;

        bool addDrawable(I_Drawable& drawable);
        void removeDrawable(I_Drawable& drawable);
        void addCollidable(I_Collidable& collidable);
        void removeCollidable(I_Collidable& collidable);

        /** Queue an event; subscribers receive it on the next dispatch pass. */
        void queueEvent(std::shared_ptr<const I_Event> event);

        /** Register a subscriber for one event type. */
        void addEventSubscriber(EventType type, I_Subscriber& subscriber);
        void removeEventSubscriber(EventType type, I_Subscriber& subscriber);
        void removeEventSubscriber(I_Subscriber& subscriber);

        template <typename TEvent>
        void addEventSubscriber(I_Subscriber& subscriber) {
            addEventSubscriber(GetEventType<TEvent>(), subscriber);
        }

        template <typename TEvent>
        void removeEventSubscriber(I_Subscriber& subscriber) {
            removeEventSubscriber(GetEventType<TEvent>(), subscriber);
        }

        /** Direct access for applications that need lower-level control. */
        DrawStack2D& getDrawStack() noexcept;
        PhysicsSystem2D& getPhysicsSystem() noexcept;
        Dispatcher& getDispatcher() noexcept;

    private:
        EngineConfig _config;
        std::atomic<bool> _running;
        std::atomic<bool> _inputsAvailable;
        std::vector<int> _inputs;
        DrawStack2D _drawStack;
        Dispatcher _dispatcher;
        PhysicsSystem2D _physicsSystem;
        std::unique_ptr<Window> _window;
    };

}

#endif // BE2D_ENGINE_HPP
