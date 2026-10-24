
#include "wrappers/KeyboardWrapper.hpp"
#include "KeyboardMock.hpp"

#include <unordered_map>
#include <unordered_set>

namespace BurgerEngine2D {

    namespace {
        std::unordered_set<Key> pressedKeys;
        std::unordered_map<Key, int> queryCounts;
    }

    bool KeyboardWrapper::isKeyPressed(Key key) {
        ++queryCounts[key];
        return pressedKeys.find(key) != pressedKeys.end();
    }

    void mockPressKey(Key key) {
        pressedKeys.insert(key);
    }

    void mockReleaseKey(Key key) {
        pressedKeys.erase(key);
    }

    void mockResetKeyboard() {
        std::unordered_set<Key>{}.swap(pressedKeys);
        std::unordered_map<Key, int>{}.swap(queryCounts);
    }

    int mockGetQueryCount(Key key) {
        const auto it = queryCounts.find(key);

        if (it == queryCounts.end()) {
            return 0;
        }

        return it->second;
    }
}
