#pragma once

#include "situation.h"

/**
 * @brief Interface for Solver (computer, man console, etc)
 */
class ISolver {
public:
    virtual std::vector<Situation> solve(const Situation& parStart) = 0;
    virtual ~ISolver() = default;

    inline static size_t N_;

    static size_t getN() {
        return N_;
    }
};