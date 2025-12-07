#pragma once

#include "situation.h"

/**
 * @brief Interface for Solver (computer, man console, etc)
 */
class ISolver {
public:
    virtual std::vector<Situation> solve(const Situation& parStart) = 0;
    virtual ~ISolver() = default;
};
