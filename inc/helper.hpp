#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H


struct greaterDepthComparer {
    template <typename T> bool operator()(const T* a, const T* b) noexcept{
    return (*a > *b);
}
};
struct lesserDepthComparer {
    template <typename T> bool operator()(const T* a, const T* b) noexcept{
    return (*a < *b);
}
};
#endif // HELPER_FUNCTIONS_H