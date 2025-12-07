#pragma once

#include <cinttypes>

/**
 * @brief Types of cells and checkers on the game board.
 */
enum class EnumCellType : uint8_t {
    /**< Empty cell. */
    CELL,
    /**< White checker piece. */
    CHECKER_WHITE,
    /**< White queen piece. */
    CHECKER_WHITE_QUEEN,
    /**< Black checker piece. */
    CHECKER_BLACK,
    /**< Black queen piece. */
    CHECKER_BLACK_QUEEN
};

/**
 * @brief Namespace containing utility functions for cell type operations.
 */
namespace CellFunc {

    /**
     * @brief Checks if a cell type represents a checker piece.
     *
     * @param parCellType The cell type to check.
     * @return true if the cell contains a checker, false otherwise.
     */
    inline bool isChecker(EnumCellType parCellType) {
        return parCellType != EnumCellType::CELL;
    }

    /**
     * @brief Checks if a cell type represents a queen piece.
     *
     * @param parCellType The cell type to check.
     * @return true if the cell contains a queen, false otherwise.
     */
    inline bool isQueen(EnumCellType parCellType) {
        return parCellType == EnumCellType::CHECKER_WHITE_QUEEN ||
               parCellType == EnumCellType::CHECKER_BLACK_QUEEN;
    }

    /**
     * @brief Checks if a cell type represents a black piece.
     *
     * @param parCellType The cell type to check.
     * @return true if the cell contains a black piece, false otherwise.
     */
    inline bool isBlack(EnumCellType parCellType) {
        return isChecker(parCellType) &&
               (parCellType == EnumCellType::CHECKER_BLACK ||
               parCellType == EnumCellType::CHECKER_BLACK_QUEEN);
    }

    /**
     * @brief Checks if a cell type represents a white piece.
     *
     * @param parCellType The cell type to check.
     * @return true if the cell contains a white piece, false otherwise.
     */
    inline bool isWhite(EnumCellType parCellType) {
        return isChecker(parCellType) && (
               parCellType == EnumCellType::CHECKER_WHITE ||
               parCellType == EnumCellType::CHECKER_WHITE_QUEEN);
    }

    /**
     * @brief Checks if two cell types represent pieces of different colors.
     *
     * @param parCellTypeFirst The first cell type to compare.
     * @param parCellTypeSecond The second cell type to compare.
     * @return true if both are checkers of different colors, false otherwise.
     */
    inline bool areDifferentColorsCheckers(EnumCellType parCellTypeFirst, EnumCellType parCellTypeSecond) {
        return isChecker(parCellTypeFirst) && isChecker(parCellTypeSecond) && ((isWhite(parCellTypeFirst) && isBlack(parCellTypeSecond)) ||
            (isBlack(parCellTypeFirst) && isWhite(parCellTypeSecond)));
    }

    /**
     * @brief Promotes a checker to a queen.
     *
     * @param parEnumCell The cell type to promote.
     * @return EnumCellType The promoted cell type (queen version).
     */
    inline EnumCellType promoteToQueen(EnumCellType parEnumCell) {
        switch (parEnumCell) {
            case EnumCellType::CHECKER_BLACK:
            case EnumCellType::CHECKER_BLACK_QUEEN:
                return EnumCellType::CHECKER_BLACK_QUEEN;
            case EnumCellType::CHECKER_WHITE:
            case EnumCellType::CHECKER_WHITE_QUEEN:
                return EnumCellType::CHECKER_WHITE_QUEEN;
            case EnumCellType::CELL:
            default:
                return EnumCellType::CELL;
        }
    }
}