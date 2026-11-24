#pragma once
#include <cmath>
#include <cstdlib>
#include <gm/a01.hpp>
#include <iostream>

inline void check(bool condition, const char *message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

inline gm::Mesh tetra() {
    gm::Mesh m;
    m.V.resize(4, 3);
    m.V << 1, 1, 1, -1, -1, 1, -1, 1, -1, 1, -1, -1;
    m.F.resize(4, 3);
    m.F << 0, 2, 1, 0, 1, 3, 0, 3, 2, 1, 2, 3;
    return m;
}

template <class F> inline void rejects(F action) {
    bool rejected = false;
    try {
        action();
    } catch (const std::invalid_argument &) {
        rejected = true;
    }
    check(rejected, "Expected invalid_argument");
}
