#include "depth_search_solver.h"

#include <algorithm>

/**
 * @brief Recursively explores possible moves using depthSearch.
 * @param parCurrentSituation - root situation (checked by target func and then generate next situations if needs)
 * @param parTargetFunc - target function (if win return true)
 * @param parDepth - current value of depth (reverse) for recursion
 * @param parTempPath - current path generated in recursion
 * @param parFoundPath - win-path (if found)
 * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
 */
void DepthSearchSolver::depthSearchRecursive(Situation &parCurrentSituation, Situation::Target parTargetFunc,
                                                    size_t parDepth, std::vector<Situation> &parTempPath, std::vector<Situation> &parFoundPath,
                                                    Situation::EvaluateScore parEvaluateScoreFunc) {

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
#if 0 //TODO
        if (std::find(parTempPath.begin() , parTempPath.end(), nextSituation) != parTempPath.end()) {
            continue;
        }
#else

        if (std::ranges::any_of(parTempPath,
                                [&](const Situation& s) { return s == nextSituation; })) {
            continue;
            }
#endif
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
 * @param parMaxDepth - max depth of search tree
 * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
 */
std::vector<Situation> DepthSearchSolver::solveDepthSearchWithScoreFunc(Situation parStart, Situation::Target parTargetFunc,
    size_t parMaxDepth, Situation::EvaluateScore parEvaluateScoreFunc) {
    std::vector<Situation> tempPath{};
    std::vector<Situation> foundPath;
    depthSearchRecursive(parStart, parTargetFunc, parMaxDepth, tempPath, foundPath, parEvaluateScoreFunc);
    return foundPath;
}

/**
 * @brief Constructs object to call fabric method
 * @param parMaxDepth - max depth of DFS
* @param parTargetFunc - target function (if win return true)
 * @param parEvaluateScoreFunc - evaluate function (helps to sort generated situations and choose the best)
 */
DepthSearchSolver::DepthSearchSolver(size_t parMaxDepth, Situation::Target parTargetFunc,
    Situation::EvaluateScore parEvaluateScoreFunc): maxDepth_(
                                             parMaxDepth), targetFunc_(parTargetFunc), evaluateScoreFunc_(parEvaluateScoreFunc) {}

/**
 * Solve DFS (with or without evaluate function)
 * @param parStart - start situation
 * @return calculated path of situations (win in the best case)
 */
std::vector<Situation> DepthSearchSolver::solve(const Situation &parStart) {
    return solveDepthSearchWithScoreFunc(parStart, targetFunc_, maxDepth_, evaluateScoreFunc_);
}