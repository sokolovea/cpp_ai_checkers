/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*             DepthSearchSolver class            */
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
    size_t maxDepth_;
    EvaluateScore evaluateScoreFunc_;
/**
 * @brief Private constructor to prevent instantiation.
 */
private:

    /**
     * @brief Recursively explores possible moves using depthSearch.
     */
    static void depthSearchRecursive(
        Situation& currentSituation,
        Target targetFunc,
        size_t depth,
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
        Situation nextSituation(false);
        for (int64_t i = 0; i < movesCount; i++) {
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
     * @brief Finds a sequence of moves using depthSearch with function evaluates best situation by score
     */
    static std::vector<Situation> solveDepthSearchWithScoreFunc(Situation start, Target target, size_t parMaxDepth,
                                                                EvaluateScore evaluateScore) {
        std::vector<Situation> tempPath{start};
        std::vector<Situation> foundPath;
        depthSearchRecursive(start, target, parMaxDepth, tempPath, foundPath, evaluateScore);
        return foundPath;
    }

public:

    explicit DepthSearchSolver(size_t parMaxDepth, EvaluateScore parEvaluateScoreFunc = nullptr) : maxDepth_(parMaxDepth), evaluateScoreFunc_(parEvaluateScoreFunc) {}

    std::vector<Situation> solve(const Situation& parStart, Target parTargetFunc) override {
        return solveDepthSearchWithScoreFunc(parStart, parTargetFunc, maxDepth_, evaluateScoreFunc_);
    }

//    /**
//     * @brief Finds a sequence of moves using minimax method
//     */
//    static std::vector<Situation> solveMinimax(const Situation& parStart, Target parTarget, size_t parMaxDepth,
//                                               EvaluateScore parEvaluateScoreFunc,
//                                               bool parIsAlphaBeta = false) {
//        auto* tree = new TreeMinimax{parStart };
//        bool parIsMin = false;
//        tree->generateTreeWithDepth(parMaxDepth, parEvaluateScoreFunc, parTarget);
//        auto foundPath = tree->solveMinimax(parIsMin, parIsAlphaBeta);
//        delete tree;
//        return foundPath;
//    }
//
//
//    /**
//     * @brief Finds a sequence of moves using mininax method
//     */
//    static std::vector<Situation> solveAlphaBetaOptimized(Situation& start, Target target, size_t parMaxDepth,
//                                                        EvaluateScore parEvaluateScoreFunc) {
//        auto* tree = new TreeMinimax{ start };
//        bool parIsMin = false;
//        auto foundPath = tree->solveAlphaBetaOptimized(parIsMin, parMaxDepth, parEvaluateScoreFunc);
//        delete tree;
//        return foundPath;
//    }
//
//    static std::vector<Situation> solveMinimaxAlphaBeta(Situation& start, Target target, size_t parMaxDepth,
//        EvaluateScore parEvaluateScoreFunc) {
//        return solveMinimax(start, target, parMaxDepth, parEvaluateScoreFunc, true);
//    }
//
//    static std::vector<Situation> solveMinimaxAlphaBetaOptimized(Situation& start, Target target, size_t parMaxDepth,
//        EvaluateScore parEvaluateScoreFunc) {
//        return solveAlphaBetaOptimized(start, target, parMaxDepth, parEvaluateScoreFunc);
//    }
//
//    static std::vector<Situation> solveManConsole(const Situation& start) {
//        Situation resultSituation(start);
//        char letterStart, letterEnd;
//        char numberStart, numberEnd;
//        std::cin >> letterStart >> numberStart >> letterEnd >> numberEnd;
//        Coordinate startCoordinate(8 - (numberStart - '0'), letterStart - 'A');
//        Coordinate endCoordinate(8 - (numberEnd - '0'), letterEnd - 'A');
//        bool isMoveCorrect = resultSituation.applyMoveIfPossible(startCoordinate, endCoordinate);
//        if (isMoveCorrect) {
//            return std::vector{resultSituation};
//        }
//        return {};
//    }
};



