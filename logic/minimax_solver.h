#pragma once


#include "i_solver.h"
#include "tree.h"

class MinimaxSolver : public ISolver {
    /**
     * @brief Private constructor to prevent instantiation.
     */
private:
    size_t maxDepth_;
    /**
     * Target function that takes the situation and returns true if win or false otherwise
     */
    Target targetFunc_;
    EvaluateScore evaluateScoreFunc_;
    bool isAlphaBeta_;

public:
    explicit MinimaxSolver(size_t parMaxDepth, Target parTargetFunc, EvaluateScore parEvaluateScoreFunc = nullptr,
                           bool parIsAlphaBeta = false) : maxDepth_(parMaxDepth),
                                                          targetFunc_(parTargetFunc),
                                                          evaluateScoreFunc_(parEvaluateScoreFunc),
                                                          isAlphaBeta_(parIsAlphaBeta) {
    }

    std::vector<Situation> solve(const Situation &parStart) override {
        if (isAlphaBeta_) {
            Situation situation(parStart);
            return solveMinimaxAlphaBetaOptimized(situation, targetFunc_,
                                                  maxDepth_, evaluateScoreFunc_);
        }
        return solveMinimax(parStart, targetFunc_,
                            maxDepth_, evaluateScoreFunc_);
    }

    /**
     * @brief Finds a sequence of moves using minimax method
     */
    static std::vector<Situation> solveMinimax(const Situation &parStart, Target parTarget, size_t parMaxDepth,
                                               EvaluateScore parEvaluateScoreFunc,
                                               bool parIsAlphaBeta = false) {
        auto *tree = new TreeMinimax{parStart};
        bool parIsMin = false;
        tree->generateTreeWithDepth(parMaxDepth, parEvaluateScoreFunc, parTarget);
        auto foundPath = tree->solveMinimax(parIsMin, parIsAlphaBeta);
        delete tree;
        return foundPath;
    }

    /**
     * @brief Finds a sequence of moves using minimax method
     */
    static std::vector<Situation> solveMinimaxAlphaBetaOptimized(Situation &parStart, Target parTarget,
                                                                 size_t parMaxDepth,
                                                                 EvaluateScore parEvaluateScoreFunc) {
        auto *tree = new TreeMinimax{parStart};
        bool parIsMin = false;
        auto foundPath = tree->solveAlphaBetaOptimized(parIsMin, parMaxDepth, parTarget, parEvaluateScoreFunc);
        delete tree;
        return foundPath;
    }
};
