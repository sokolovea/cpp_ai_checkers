#pragma once

#include <vector>
#include <algorithm>
#include <limits>

/**
 * Evaluates the score of situation and helps to find the best next step.
 */

template<typename T>
class TreeMinimax final {

    // Evaluate score function
    using EvaluateScore = int64_t(*)(const T &);

    // Situation
    T data_;

    // Score for current part of tree (with situation)
    int64_t score_;

    // Pointer to parent part of tree
    const T *parent_;

    // Array of children leafs for current part of tree
    std::vector<TreeMinimax> children_;

    // Finds score for data_ by minimax algorithm
    void findMinimaxScore(bool parIsMin);

    // Finds score for data_ by alpha-beta prune algorithm
    void findMinimaxScoreAlphaBeta(bool parIsMin, int64_t parAlpha, int64_t parBeta);

    // Finds score for data_ by alpha-beta prune algorithm with optimizations (without full bush generation)
    void findMinimaxScoreAlphaBetaOptimized(bool parIsMin, int64_t parAlpha, int64_t parBeta, size_t parDepth,
                                            Situation::Target parTarget, EvaluateScore parEvaluateScore);

public:
    /**
    * @brief Generates Tree with selected root value and it's parent
    * @param parRoot - selected root
    * @param parParent - parent of root (if exists)
    */
    explicit TreeMinimax(const T &parRoot, T *parParent = nullptr);

    /**
    * @brief Generates tree with selected set depth
    * @param parDepth - selected depth
    * @param parEvaluateScore - evaluate function (evaluates score of situation)
    * @param parTarget - situation is target or not (win - true, loose/draw/playing - false)
    */
    void generateTreeWithDepth(size_t parDepth, EvaluateScore parEvaluateScore, Situation::Target parTarget);

    /**
     * @brief Solves minimax tree and returns recommended path with situations
     * @param parIsMin - is minimizing for this leaf (of maximizing else)
     * @param parIsAlphaBeta - is using alpha-beta prune optimizations
     */
    std::vector<T> solveMinimax(bool parIsMin, bool parIsAlphaBeta = false);

    /**
     * @brief Solves alpha-beta prune algorithm and returns recommended path with situations
     * @param parIsMin - is minimizing for this leaf (of maximizing else)
     * @param parDepth - max depth for tree
     * @param parTarget - target function (win or not now)
     * @param parEvaluateScore - evaluate situation function
     */
    std::vector<T>
    solveAlphaBetaOptimized(bool parIsMin, size_t parDepth, Situation::Target parTarget, EvaluateScore parEvaluateScore);

    /**
     * @brief Get internal data value.
     *
     * @return internal data value.
     */
    const T &getData() const;
};

/**
* @brief Generates Tree with selected root value and it's parent
* @param parRoot - selected root
* @param parParent - parent of root (if exists)
*/
template<typename T>
TreeMinimax<T>::TreeMinimax(const T &parRoot, T *parParent):
    data_{parRoot},
    score_{0},
    parent_{parParent} {}

/**
 * @brief Get internal data value.
 *
 * @return internal data value.
 */
template<typename T>
const T & TreeMinimax<T>::getData() const {
    return data_;
}

template<typename T>
void TreeMinimax<T>::findMinimaxScore(bool parIsMin) {
    if (children_.size() != 0) {
        int64_t calculatedScore = parIsMin ? std::numeric_limits<int64_t>::max() : std::numeric_limits<int64_t>::min();
        for (auto &each: children_) {
            each.findMinimaxScore(!parIsMin);
            calculatedScore = parIsMin ? std::min(calculatedScore, each.score_) : std::max(calculatedScore,
                                                                                           each.score_);
        }
        score_ = calculatedScore;
    }
}

template<typename T>
void TreeMinimax<T>::findMinimaxScoreAlphaBetaOptimized(bool parIsMin, int64_t parAlpha, int64_t parBeta,
                                                        size_t parDepth, Situation::Target parTarget, EvaluateScore parEvaluateScore) {
    if (parDepth == 0 || parTarget(this->data_)) {
        score_ = parEvaluateScore(this->data_);
    } else {
        size_t situationsCount = data_.possibleMovesCount();
        if (situationsCount >= 1) {
            children_.reserve(situationsCount);
            int64_t bestScore = parIsMin ? std::numeric_limits<int64_t>::max() : std::numeric_limits<int64_t>::min();
            for (size_t i = 0; i < situationsCount; i++) {
                children_.push_back(static_cast<TreeMinimax<Situation>>(data_.generateNextSituation()));

                auto &child = children_[children_.size() - 1];
                child.findMinimaxScoreAlphaBetaOptimized(!parIsMin, parAlpha, parBeta, parDepth - 1, parTarget, parEvaluateScore);

                if (parIsMin) {
                    bestScore = std::min(bestScore, child.score_);
                    parBeta = std::min(parBeta, bestScore);
                } else {
                    bestScore = std::max(bestScore, child.score_);
                    parAlpha = std::max(parAlpha, bestScore);
                }

                if (parAlpha >= parBeta) {
                    break;
                }
            }
            score_ = bestScore;
        } else {
            score_ = parEvaluateScore(this->data_);
        }
    }
}

template<typename T>
void TreeMinimax<T>::findMinimaxScoreAlphaBeta(bool parIsMin, int64_t parAlpha, int64_t parBeta) {
    if (!children_.empty()) {
        int64_t bestScore = parIsMin ? std::numeric_limits<int64_t>::max() : std::numeric_limits<int64_t>::min();

        for (auto &child: children_) {
            child.findMinimaxScoreAlphaBeta(!parIsMin, parAlpha, parBeta);

            if (parIsMin) {
                bestScore = std::min(bestScore, child.score_);
                parBeta = std::min(parBeta, bestScore);
            } else {
                bestScore = std::max(bestScore, child.score_);
                parAlpha = std::max(parAlpha, bestScore);
            }

            if (parAlpha >= parBeta) {
                break;
            }
        }

        score_ = bestScore;
    }
}


/**
* @brief Generates tree with selected set depth
* @param parDepth - selected depth
* @param parEvaluateScore - evaluate function (evaluates score of situation)
* @param parTarget - situation is target or not (win - true, loose/draw/playing - false)
*/
template<typename T>
void TreeMinimax<T>::generateTreeWithDepth(size_t parDepth, EvaluateScore parEvaluateScore, Situation::Target parTarget) {
    if (parDepth == 0 || parTarget(data_)) {
        score_ = parEvaluateScore(data_);
        return;
    };
    auto next_states = data_.generateAllNextSituations();
    if (next_states.size() == 0) {
        score_ = parEvaluateScore(data_);
        return;
    }
    for (const auto &state: next_states) {
        children_.emplace_back(TreeMinimax{T{state}, &data_});
    }

    std::sort(children_.begin(), children_.end(), [](const TreeMinimax &first, const TreeMinimax &second) {
        return first.score_ > second.score_;
    });

    for (auto &child: children_) {
        child.generateTreeWithDepth(parDepth - 1, parEvaluateScore, parTarget);
    }
}

/**
* @brief Solves minimax
* @param parIsMin - is minimazing (maximazing else)
* @param parIsAlphaBeta - is alpha-beta pruning optimizations used
*/
template<typename T>
std::vector<T> TreeMinimax<T>::solveMinimax(bool parIsMin, bool parIsAlphaBeta) {
    if (parIsAlphaBeta) {
        findMinimaxScoreAlphaBeta(parIsMin, std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
    } else {
        findMinimaxScore(parIsMin);
    }
    std::vector<T> path = {};
    auto nextLevelChildren = &children_;
    while (nextLevelChildren != nullptr && nextLevelChildren->size() != 0) {
        for (auto &each: *nextLevelChildren) {
            if (each.score_ == score_) {
                path.push_back(each.data_);
                nextLevelChildren = &(each.children_);
                break;
            }
        }
    }
    return path;
}


/**
* @brief Solves minimax
* @param parIsMin - is minimizing (maximizing else)
* @param parIsAlphaBeta - is alpha-beta pruning optimizations used
*/
template<typename T>
std::vector<T> TreeMinimax<T>::solveAlphaBetaOptimized(bool parIsMin, size_t parDepth, Situation::Target parTarget,
                                                       EvaluateScore parEvaluateScore) {
    findMinimaxScoreAlphaBetaOptimized(parIsMin, std::numeric_limits<int64_t>::min(),
                                       std::numeric_limits<int64_t>::max(), parDepth, parTarget, parEvaluateScore);
    std::vector<T> path = {};
    auto nextLevelChildren = &children_;
    while (nextLevelChildren != nullptr && nextLevelChildren->size() != 0) {
        for (auto &each: *nextLevelChildren) {
            if (each.score_ == score_) {
                path.push_back(each.data_);
                nextLevelChildren = &(each.children_);
                break;
            }
        }
    }
    return path;
}

