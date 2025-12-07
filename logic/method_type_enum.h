#pragma once

/**
 * @brief Developed AI methods
 */
enum class MethodTypeEnum {
    /**
     * Depth Search (non-effective but low RAM usage)
     */
    DFS = 1,
    /**
    * Depth Search with evaluation function (better than DFS, also low RAM usage)
    */
    DFS_WITH_EVALUATION_FUNC,
    /**
     * Minimax method (1926, 1945), big RAM usage but the best effective
     */
    MINIMAX,
    /**
     * Alpha-beta pruning method (1958, 1975), less RAM usage than minimax but with the same effectiveness
     */
    ALPHA_BETA_PRUNING,
};