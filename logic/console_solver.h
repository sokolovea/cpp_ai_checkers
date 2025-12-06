#pragma once

#include "i_solver.h"

/**
 * @brief Console-based solver for manual move input.
 * Allows users to input moves directly through the console interface.
 */
class ConsoleSolver: public ISolver {
public:
    /**
     * @brief Gets next situation by user input.
     * Reads move coordinates from console and applies them to the current situation.
     *
     * @param parStart Initial game situation
     * @return Vector containing the resulting situation if move is valid,
     *         empty vector otherwise
     */
    std::vector<Situation> solve(const Situation &parStart) override {
        Situation resultSituation(parStart);
        char letterStart, letterEnd;
        char numberStart, numberEnd;
        std::cin >> letterStart >> numberStart >> letterEnd >> numberEnd;
        const Coordinate startCoordinate(8 - (numberStart - '0'), letterStart - 'A');
        const Coordinate endCoordinate(8 - (numberEnd - '0'), letterEnd - 'A');
        if (resultSituation.applyMoveIfPossible(startCoordinate, endCoordinate)) {
            return std::vector{resultSituation};
        }
        return {};
    }
};
