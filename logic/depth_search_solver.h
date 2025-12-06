/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*        DepthSearchSolver class        */
/*     Developer: Sokolov Egor, 543M     */
/*          Version: 29.11.2025          */
/*****************************************/

#pragma once

#include <vector>
#include <algorithm>
#include "tree.h"
#include "situation.h"
#include "i_solver.h"

/**
 * @brief AI solver for finding a sequence of moves using depthSearch.
 */
class DepthSearchSolver : public ISolver {
    /**
     * Max depth for search tree
     */
    size_t maxDepth_;
    /**
     * Target function that takes the situation and returns true if win or false otherwise
     */
    Target targetFunc_;
    /**
     * Evaluate function (nullptr if not required)
     */
    EvaluateScore evaluateScoreFunc_;

private:

    /**
     * @brief Recursively explores possible moves using depthSearch.
     * @param parCurrentSituation - root situation (checked by target func and then generate next situations if needs)
     * @param parTargetFunc - target function (if win return true)
     * @param parDepth - current value of depth (reverse) for recursion
     * @param parTempPath - current path generated in recursion
     * @param parFoundPath - win-path (if found)
     * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
     */
    static void depthSearchRecursive(
            Situation &parCurrentSituation,
            Target parTargetFunc,
            size_t parDepth,
            std::vector<Situation> &parTempPath,
            std::vector<Situation> &parFoundPath,
            EvaluateScore parEvaluateScoreFunc = nullptr
    ) {

        if (!parFoundPath.empty()) {
            return;
        }

        bool isTarget = parTargetFunc(parCurrentSituation);

        if (isTarget) {
            parFoundPath = parTempPath;
            return;
        }

        if (parDepth == 0) {
            return;
        }

        int64_t movesCount = parCurrentSituation.possibleMovesCount();
        Situation nextSituation(false);
        for (int64_t i = 0; i < movesCount; i++) {
            if (parEvaluateScoreFunc == nullptr) {
                nextSituation = parCurrentSituation.generateNextSituation();
            } else {
                nextSituation = parCurrentSituation.generateNextBestSituation(parEvaluateScoreFunc);
            }

            if (nextSituation == parCurrentSituation) {
                continue;
            }

            if (std::find(parTempPath.begin(), parTempPath.end(), nextSituation) != parTempPath.end()) {
                continue;
            }

            parTempPath.push_back(nextSituation);
            depthSearchRecursive(nextSituation, parTargetFunc, parDepth - 1, parTempPath, parFoundPath,
                                 parEvaluateScoreFunc);
            parTempPath.pop_back();
            if (!parFoundPath.empty()) return;
        }
    }

    /**
     * @brief Finds a sequence of moves using depthSearch with function evaluates best situation by score.
     * @param parStart - start situation
     * @param parTargetFunc - target function (if win return true)
     * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
     */
    static std::vector<Situation>
    solveDepthSearchWithScoreFunc(Situation parStart, Target parTargetFunc, size_t parMaxDepth,
                                  EvaluateScore parEvaluateScoreFunc) {
        std::vector<Situation> tempPath{};
        std::vector<Situation> foundPath;
        depthSearchRecursive(parStart, parTargetFunc, parMaxDepth, tempPath, foundPath, parEvaluateScoreFunc);
        return foundPath;
    }

public:
    /**
     * @brief Constructs object to call fabric method
     * @param parMaxDepth - max depth of DFS
     * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
     */
    explicit DepthSearchSolver(size_t parMaxDepth, Target parTargetFunc, EvaluateScore parEvaluateScoreFunc = nullptr) : maxDepth_(
            parMaxDepth), targetFunc_(parTargetFunc), evaluateScoreFunc_(parEvaluateScoreFunc) {}

    /**
     * Solve DFS (with or without evaluate function)
     * @param parStart - start situation
     * @param parTargetFunc - target function (if win return true)
     * @return calculated path of situations (win in the best case)
     */
    std::vector<Situation> solve(const Situation &parStart) override {
        return solveDepthSearchWithScoreFunc(parStart, targetFunc_, maxDepth_, evaluateScoreFunc_);
    }
};



