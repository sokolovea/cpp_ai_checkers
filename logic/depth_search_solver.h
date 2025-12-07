#pragma once

#include <vector>
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
    Situation::Target targetFunc_;
    /**
     * Evaluate function (nullptr if not required)
     */
    Situation::EvaluateScore evaluateScoreFunc_;

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
            Situation::Target parTargetFunc,
            size_t parDepth,
            std::vector<Situation> &parTempPath,
            std::vector<Situation> &parFoundPath,
            Situation::EvaluateScore parEvaluateScoreFunc = nullptr
    );

    /**
     * @brief Finds a sequence of moves using depthSearch with function evaluates best situation by score.
     * @param parStart - start situation
     * @param parTargetFunc - target function (if win return true)
     * @param parMaxDepth - max depth of search tree
     * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
     */
    static std::vector<Situation>
    solveDepthSearchWithScoreFunc(Situation parStart, Situation::Target parTargetFunc, size_t parMaxDepth,
                                  Situation::EvaluateScore parEvaluateScoreFunc);

public:
    /**
     * @brief Constructs object to call fabric method
     * @param parMaxDepth - max depth of DFS
    * @param parTargetFunc - target function (if win return true)
     * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
     */
    explicit DepthSearchSolver(size_t parMaxDepth, Situation::Target parTargetFunc, Situation::EvaluateScore parEvaluateScoreFunc = nullptr);

    /**
     * Solve DFS (with or without evaluate function)
     * @param parStart - start situation
     * @return calculated path of situations (win in the best case)
     */
    std::vector<Situation> solve(const Situation &parStart) override;
};

