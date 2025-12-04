#pragma once

#include "situation.h"
/**
 * @brief Interface for Solver (computer, man console, etc)
 */
class ISolver {
    virtual std::vector<Situation> solve() = 0;
};
