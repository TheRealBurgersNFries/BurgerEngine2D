#include "wrappers/KeyboardWrapper.hpp"
#include "KeyboardMock.hpp"

#include <unordered_map>
#include <unordered_set>

namespace BurgerEngine2D {

    namespace {
        std::unordered_set<int> pressedKeys;
        std::unordered_map<int, int> queryCounts;
    }

    bool KeyboardWrapper::isKeyPressed(int key) {
        ++queryCounts[key];
        return pressedKeys.find(key) != pressedKeys.end();
    }

    void mockPressKey(int key) {
        pressedKeys.insert(key);
    }

    void mockReleaseKey(int key) {
        pressedKeys.erase(key);
    }

    void mockResetKeyboard() {
        pressedKeys.clear();
        queryCounts.clear();
    }

    int mockGetQueryCount(int key) {
        const auto it = queryCounts.find(key);

        if (it == queryCounts.end()) {
            return 0;
        }

        return it->second;
    }
}