#pragma once

#include "i_solver.h"

class ConsoleSolver: public ISolver {
public:
    std::vector<Situation> solve(const Situation &parStart, Target parTargetFunc) override {
        Situation resultSituation(parStart);
        char letterStart, letterEnd;
        char numberStart, numberEnd;
        std::cin >> letterStart >> numberStart >> letterEnd >> numberEnd;
        Coordinate startCoordinate(8 - (numberStart - '0'), letterStart - 'A');
        Coordinate endCoordinate(8 - (numberEnd - '0'), letterEnd - 'A');
        bool isMoveCorrect = resultSituation.applyMoveIfPossible(startCoordinate, endCoordinate);
        if (isMoveCorrect) {
            return std::vector{resultSituation};
        }
        return {};
    }
};
