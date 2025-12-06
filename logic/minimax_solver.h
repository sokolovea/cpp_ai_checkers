#pragma once


#include "i_solver.h"
#include "tree.h"

/**
 * @brief Implementation of Minimax algorithm with optional Alpha-Beta pruning.
 */
class MinimaxSolver : public ISolver {

    /**
     * @brief Maximum depth for the search tree.
     */
    size_t maxDepth_;

    /**
     * @brief Target function to check winning condition.
     * Returns true if the given situation is a winning one.
     */
    Target targetFunc_;

    /**
     * @brief Evaluation function for scoring situations.
     */
    EvaluateScore evaluateScoreFunc_;

    /**
     * @brief Flag indicating whether to use Alpha-Beta pruning.
     */
    bool isAlphaBeta_;

public:
    /**
     * @brief Constructor for MinimaxSolver.
     * Initializes solver with specified parameters.
     *
     * @param parMaxDepth Maximum depth of the search tree
     * @param parTargetFunc Function to check winning condition
     * @param parEvaluateScoreFunc Optional evaluation function
     * @param parIsAlphaBeta Flag to enable Alpha-Beta pruning
     */
    explicit MinimaxSolver(size_t parMaxDepth, Target parTargetFunc, EvaluateScore parEvaluateScoreFunc = nullptr,
                           bool parIsAlphaBeta = false) : maxDepth_(parMaxDepth),
                                                          targetFunc_(parTargetFunc),
                                                          evaluateScoreFunc_(parEvaluateScoreFunc),
                                                          isAlphaBeta_(parIsAlphaBeta) {
    }

    /**
     * @brief Main solving method.
     * Selects and executes appropriate solving algorithm based on configuration.
     *
     * @param parStart Initial game situation
     * @return Vector of situations representing the solution path
     */
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
     * @brief Implements classic Minimax algorithm.
     * Constructs a search tree and finds the optimal move sequence.
     *
     * @param parStart Initial game situation
     * @param parTarget Target function for winning condition
     * @param parMaxDepth Maximum search depth
     * @param parEvaluateScoreFunc Evaluation function
     * @param parIsAlphaBeta Flag to enable Alpha-Beta pruning
     * @return Vector of situations representing the solution path
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
     * @brief Implements Minimax algorithm with Alpha-Beta pruning.
     * Optimized version of the algorithm with search space reduction.
     *
     * @param parStart Initial game situation
     * @param parTarget Target function for winning condition
     * @param parMaxDepth Maximum search depth
     * @param parEvaluateScoreFunc Evaluation function
     * @return Vector of situations representing the solution path
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
