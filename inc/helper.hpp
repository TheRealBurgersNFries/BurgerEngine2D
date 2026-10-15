#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H


struct greaterDepthComparer {
    template <typename T> bool operator()(const T* a, const T* b) const noexcept {
    if (*a > *b)
        return true;
    if (*a < *b)
        return false;
    return std::less<const T*>{}(b, a);
}
};
struct lesserDepthComparer {
    template <typename T> bool operator()(const T* a, const T* b) const noexcept {
    if (*a < *b)
        return true;
    if (*a > *b)
        return false;
    return std::less<const T*>{}(a, b);
}
};
#endif // HELPER_FUNCTIONS_H