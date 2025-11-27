/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*        EnumGameCurrentStatus          */
/*      Developer: Sokolov Egor, 543M    */
/*           Version: 04.11.2025         */
/*****************************************/

#ifndef CHECKERS_ENUMGAMEEND_H
#define CHECKERS_ENUMGAMEEND_H

/**
 * @brief Enumeration representing the current status of the game.
 */
enum class EnumGameCurrentStatus {
    WHITE_VICTORY,  /**< White player has won the game. */
    BLACK_VICTORY,  /**< Black player has won the game. */
    PLAYING,        /**< Game is currently in progress. */
    DRAW,           /**< Game ended in a draw. */
};

#endif //CHECKERS_ENUMGAMEEND_H