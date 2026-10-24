#ifndef KEYBOARD_CONTROLLER_HPP
#define KEYBOARD_CONTROLLER_HPP


#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>

#include "KeyboardWrapper.hpp"
#include "KeyboardKey.hpp"

namespace BurgerEngine2D {
    using Key = BurgerEngine2D::KeyboardKey;
    
    
    class KeyboardController {
        
        public:
            using KeyCallback = std::function<void(Key)>;
            using SubscriptionId = std::size_t;

            explicit KeyboardController(KeyboardWrapper& keyboard);
            
            SubscriptionId subscribe(Key key, KeyCallback callback);
            void unsubscribe(SubscriptionId id);
            
            void update();
        
        private:
            struct Listener {
                SubscriptionId id;
                KeyCallback callback;
            };
            
            KeyboardWrapper& _keyboard;
            std::unordered_map<Key, std::vector<Listener>> _keyRegistry;
            SubscriptionId _nextId = 1;
        
    };
}


#endif // KEYBOARD_CONTROLLER_HPP
