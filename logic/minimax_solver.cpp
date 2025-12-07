#include "minimax_solver.h"


/**
 * @brief Constructor for MinimaxSolver.
 * Initializes solver with specified parameters.
 *
 * @param parMaxDepth Maximum depth of the search tree
 * @param parTargetFunc Function to check winning condition
 * @param parEvaluateScoreFunc Optional evaluation function
 * @param parIsAlphaBeta Flag to enable Alpha-Beta pruning
 */
MinimaxSolver::MinimaxSolver(size_t parMaxDepth, Situation::Target parTargetFunc,
    Situation::EvaluateScore parEvaluateScoreFunc,
    bool parIsAlphaBeta): maxDepth_(parMaxDepth),
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
std::vector<Situation> MinimaxSolver::solve(const Situation &parStart) {
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
std::vector<Situation> MinimaxSolver::solveMinimax(const Situation &parStart, Situation::Target parTarget,
    size_t parMaxDepth, Situation::EvaluateScore parEvaluateScoreFunc, bool parIsAlphaBeta) {
    auto *tree = new TreeMinimax{parStart};
    bool parIsMin = false;
    tree->generateTreeWithDepth(parMaxDepth, parEvaluateScoreFunc, parTarget);
    N_ = TreeMinimax<Situation>::getN();
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
std::vector<Situation> MinimaxSolver::solveMinimaxAlphaBetaOptimized(Situation &parStart, Situation::Target parTarget,
    size_t parMaxDepth, Situation::EvaluateScore parEvaluateScoreFunc) {
    auto *tree = new TreeMinimax{parStart};
    bool parIsMin = false;
    auto foundPath = tree->solveAlphaBetaOptimized(parIsMin, parMaxDepth, parTarget, parEvaluateScoreFunc);
    N_ = TreeMinimax<Situation>::getN();
    delete tree;
    return foundPath;
}