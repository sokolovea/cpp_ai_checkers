#include <vector>
#include <cinttypes>
#include <algorithm>
#include <climits>

/**
 * Evaluates the score of situation and helps to find the best next step.
 */
typedef int64_t(*EvaluateScore)(const Situation&);

template <typename T>
class TreeMinimax final {
    // Situation
    T data_;

    // Score for current part of tree (with situation)
    int64_t score_;

    // Pointer to parent part of tree
    const T* parent_;

    // Array of children leafs for current part of tree
    std::vector<TreeMinimax> children_;

    // Finds score for data_ by minimax algorithm
    void findMinimaxScore(bool parIsMin);

    // Finds score for data_ by minimax algorithm
    void findMinimaxScoreAlphaBeta(bool parIsMin, int64_t parAlpha, int64_t parBeta);
public:

    /**
    * @brief Generates Tree with selected root value and it's parent
    * @param parRoot - selected root
    * @param parParent - parent of root (if exists)
    */
    TreeMinimax(const T& parRoot, T* parParent = nullptr) :
        data_{ parRoot },
        score_ { 0 },
        parent_ { parParent } { }

    /**
     * @brief Generates tree with set depth
     * @param parDepth - max tree depth
     * @param parEvaluateScore - evaluate score function 
     */
    void generateTreeWithDepth(size_t parDepth, EvaluateScore parEvaluateScore);

    /**
     * @brief Solves minimax tree and returns recommended path with situations
     * @param parIsMin - is minimazing for this leaf (of maximazing else)
     * @param parIsAplhaBeta - is using alpha-beta prune optimizations
     */
    std::vector<T> solveMinimax(bool parIsMin, bool parIsAplhaBeta = false);

    const T& getData() const {
        return data_;
    }

};

template<typename T>
void TreeMinimax<T>::findMinimaxScore(bool parIsMin)
{
    if (children_.size() != 0) {
        int64_t calculatedScore = parIsMin ? INT_MAX : INT_MIN;
        for (auto& each : children_) {
            each.findMinimaxScore(!parIsMin);
            calculatedScore = parIsMin ? std::min(calculatedScore, each.score_) : std::max(calculatedScore, each.score_);
        }
        score_ = calculatedScore;
    }
}

template<typename T>
void TreeMinimax<T>::findMinimaxScoreAlphaBeta(bool parIsMin, int64_t parAlpha, int64_t parBeta) {
    if (!children_.empty()) {
        int64_t bestScore = parIsMin ? std::numeric_limits<int64_t>::max() : std::numeric_limits<int64_t>::min();

        for (auto& child : children_) {
            child.findMinimaxScoreAlphaBeta(!parIsMin, parAlpha, parBeta);

            if (parIsMin) {
                bestScore = std::min(bestScore, child.score_);
                parBeta = std::min(parBeta, bestScore);
            }
            else {
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
* @param depth - selected depth
*/
template<typename T>
void TreeMinimax<T>::generateTreeWithDepth(size_t parDepth, EvaluateScore parEvaluateScore) {
    if (parDepth == 0) {
        score_ = parEvaluateScore(data_);
        return;
    };

    auto next_states = data_.generateAllNextSituations();
    if (next_states.size() == 0) {
        score_ = parEvaluateScore(data_);
        return;
    }
    for (const auto& state : next_states) {
        children_.emplace_back(TreeMinimax{ T{state}, &data_ });
    }

    std::sort(children_.begin(), children_.end(), [](const TreeMinimax& first, const TreeMinimax& second) {
            return first.score_ > second.score_;
        });

    for (auto& child : children_) {
        child.generateTreeWithDepth(parDepth - 1, parEvaluateScore);
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
        findMinimaxScoreAlphaBeta(parIsMin, INT_MIN, INT_MAX);
    } else {
        findMinimaxScore(parIsMin);
    }
    std::vector<T> path = {};
    auto nextLevelChildren = &children_;
    while (nextLevelChildren != nullptr && nextLevelChildren->size() != 0) {
        for (auto& each : *nextLevelChildren) {
            if (each.score_ == score_) {
                path.push_back(each.data_);
                nextLevelChildren = &(each.children_);
                break;
            }
        }
    }
    return path;
}
