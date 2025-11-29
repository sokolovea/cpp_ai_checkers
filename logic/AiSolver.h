/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*             AiSolver class            */
/*     Developer: Sokolov Egor, 543M     */
/*          Version: 29.11.2025          */
/*****************************************/

#pragma once

#include <vector>
#include <algorithm>
#include "Tree.h"
#include "Situation.h"

/**
 * @brief AI solver for finding a sequence of moves using depthSearch.
 */
class AiSolver {
private:
    /**
     * @brief Private constructor to prevent instantiation.
     */
    explicit AiSolver() {}

    /**
     * @brief Recursively explores possible moves using depthSearch.
     */
    static void depthSearchRecursive(
        Situation& currentSituation,
        Target targetFunc,
        int64_t depth,
        std::vector<Situation>& tempPath,
        std::vector<Situation>& foundPath,
        EvaluateScore evaluateScoreFunc = nullptr
    ) {

        if (!foundPath.empty()) {
            return;
        }
        
        int64_t value = targetFunc(currentSituation);

        if (value > 0) {
            foundPath = tempPath;
            return;
        }

        if (depth == 0) {
            return;
        }

        int64_t movesCount = currentSituation.possibleMovesCount();
        for (int64_t i = 0; i < movesCount; i++) {
            Situation nextSituation(false);
            if (evaluateScoreFunc == nullptr) {
                nextSituation = currentSituation.generateNextSituation();
            } else {
                nextSituation = currentSituation.generateNextBestSituation(evaluateScoreFunc);
            }

            if (nextSituation == currentSituation) { 
                continue;
            }

            if (std::find(tempPath.begin(), tempPath.end(), nextSituation) != tempPath.end()) {
                continue;
            }

            tempPath.push_back(nextSituation);
            depthSearchRecursive(nextSituation, targetFunc, depth - 1, tempPath, foundPath, evaluateScoreFunc);
            tempPath.pop_back();

            if (!foundPath.empty()) return;
        }
    }

    /**
     * @brief Recursively explores possible moves using breadthSearch.
     */
    static void breadthSearchMinimaxRecursive(
        Situation& currentSituation,
        Target targetFunc,
        int64_t depth,
        std::vector<Situation>& tempPath,
        std::vector<Situation>& foundPath,
        EvaluateScore evaluateScoreFunc = nullptr
    ) {

        if (!foundPath.empty()) {
            return;
        }
        
        int64_t value = targetFunc(currentSituation);

        if (value > 0) {
            foundPath = tempPath;
            return;
        }

        if (depth == 0) {
            return;
        }

        int64_t movesCount = currentSituation.possibleMovesCount();
        for (int64_t i = 0; i < movesCount; i++) {
            Situation nextSituation(false);
            if (evaluateScoreFunc == nullptr) {
                nextSituation = currentSituation.generateNextSituation();
            } else {
                nextSituation = currentSituation.generateNextBestSituation(evaluateScoreFunc);
            }

            if (nextSituation == currentSituation) { 
                continue;
            }

            if (std::find(tempPath.begin(), tempPath.end(), nextSituation) != tempPath.end()) {
                continue;
            }

            tempPath.push_back(nextSituation);
            depthSearchRecursive(nextSituation, targetFunc, depth - 1, tempPath, foundPath, evaluateScoreFunc);
            tempPath.pop_back();

            if (!foundPath.empty()) return;
        }
    }

public:
    /**
     * @brief Finds a sequence of moves using depthSearch.
     */
    static std::vector<Situation> solvedepthSearch(Situation start, Target target, int64_t maxDepth) {
        std::vector<Situation> tempPath{start};
        std::vector<Situation> foundPath;
        depthSearchRecursive(start, target, maxDepth, tempPath, foundPath);
        return foundPath;
    }

    /**
     * @brief Finds a sequence of moves using depthSearch with function evaluates best situation by score
     */
    static std::vector<Situation> solvedepthSearchWithScoreFunc(Situation start, Target target, int64_t maxDepth,
                                                        EvaluateScore evaluateScore) {
        std::vector<Situation> tempPath{start};
        std::vector<Situation> foundPath;
        depthSearchRecursive(start, target, maxDepth, tempPath, foundPath, evaluateScore);
        return foundPath;
    }

    /**
     * @brief Finds a sequence of moves using mininax method
     */
    static std::vector<Situation> solveMinimax(Situation start, Target target, int64_t maxDepth,
                                                        EvaluateScore parEvaluateScoreFunc,
                                                                bool parIsAlphaBeta = false) {
        TreeMinimax<Situation>* tree = new TreeMinimax{ start };
        bool parIsMin = false;
        tree->generateTreeWithDepth(maxDepth, parEvaluateScoreFunc);
        auto foundPath = tree->solveMinimax(parIsMin, parIsAlphaBeta);
        delete tree;
        return foundPath;
    }


    /**
     * @brief Finds a sequence of moves using mininax method
     */
    static std::vector<Situation> solveAlphaBetaOptimized(Situation& start, Target target, int64_t maxDepth,
                                                        EvaluateScore parEvaluateScoreFunc) {
        TreeMinimax<Situation>* tree = new TreeMinimax{ start };
        bool parIsMin = false;
        auto foundPath = tree->solveAlphaBetaOptimized(parIsMin, maxDepth - 1, parEvaluateScoreFunc);
        delete tree;
        return foundPath;
    }

    static std::vector<Situation> solveMinimaxAlphaBeta(Situation& start, Target target, int64_t maxDepth,
        EvaluateScore parEvaluateScoreFunc) {
        return solveMinimax(start, target, maxDepth, parEvaluateScoreFunc, true);
    }

    static std::vector<Situation> solveMinimaxAlphaBetaOptimized(Situation& start, Target target, int64_t maxDepth,
        EvaluateScore parEvaluateScoreFunc) {
        return solveAlphaBetaOptimized(start, target, maxDepth, parEvaluateScoreFunc);
    }
};



