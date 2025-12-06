#pragma once

#include "situation.h"
/**
 * @brief Interface for Solver (computer, man console, etc)
 */
class ISolver {
public:
    virtual std::vector<Situation> solve(const Situation& parStart, Target parTargetFunc) = 0;
    virtual ~ISolver() = default;
};
