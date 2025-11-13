/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*               AiSolver class          */
/*     Developer: Sokolov Egor, 543M     */
/*          Version: 04.11.2025          */
/*****************************************/

#include <vector>
#include <algorithm>
#include "Tree.h"
#include "Situation.h"

/**
 * Target function that evaluates the situation and returns current score of player.
 */
typedef bool (*Target)(const Situation&);

/**
 * Evaluates the score of situation and helps to find the best next step.
 */
typedef int64_t (*EvaluateScore)(const Situation&);

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
        std::vector<Situation> foundPath{start};
        TreeMinimax<Situation>* tree = new TreeMinimax{ start };
        bool parIsMin = false;
        while (1) {
            tree->generateTreeWithDepth(maxDepth, parEvaluateScoreFunc);
            auto newPartOfPath = tree->solveMinimax(parIsMin, parIsAlphaBeta);

            if (newPartOfPath.size() == 0) {
                break;
            }
#ifdef DEBUG
            std::cout << "DEBUG!\n";
            newPartOfPath[0].printField();
            std::cout << "\n\n\n";
#endif

            foundPath.push_back(newPartOfPath[0]);
            parIsMin = !parIsMin;
            delete tree;
            tree = new TreeMinimax(foundPath[foundPath.size() - 1]);
        }
        delete tree;
        return foundPath;
    }


    /**
     * @brief Finds a sequence of moves using mininax method
     */
    static std::vector<Situation> solveAlphaBetaOptimized(Situation& start, Target target, int64_t maxDepth,
                                                        EvaluateScore parEvaluateScoreFunc) {
        std::vector<Situation> foundPath{start};
        TreeMinimax<Situation>* tree = new TreeMinimax{ start };
        bool parIsMin = false;
        while (1) {
            auto newPartOfPath = tree->solveAlphaBetaOptimized(parIsMin, maxDepth - 1, parEvaluateScoreFunc);

            if (newPartOfPath.size() == 0) {
                break;
            }
#ifdef DEBUG
            std::cout << "DEBUG!\n";
            newPartOfPath[0].printField();
            std::cout << "\n\n\n";
#endif

            foundPath.push_back(newPartOfPath[0]);
            parIsMin = !parIsMin;
            delete tree;
            tree = new TreeMinimax(foundPath[foundPath.size() - 1]);
        }
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



