/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*               Cell class              */
/*     Developer: Sokolov Egor, 543M     */
/*          Version: 04.11.2025          */
/*****************************************/

#pragma once

#include "enum_cell_type.h"

/**
 * @brief Represents a single cell on the checkers board.
 */
class Cell {
public:

    /**
     * @brief Creates an invalid cell with default CELL type.
     */
    Cell() = default;

    /**
     * @brief Creates a valid cell with the specified cell type.
     * @param parCellType The type of cell to create.
     */
    explicit Cell(EnumCellType parCellType) {
        cellType_ = parCellType;
        valid_ = true;
    }

    /**
     * @brief Checks if the cell is valid.
     */
    [[nodiscard]] bool isValid() const {
        return valid_;
    }

    /**
     * @brief Gets the type of the cell.
     */
    [[nodiscard]] EnumCellType cellType() const {
        return cellType_;
    }

    /**
     * @brief Converts a cell enum value to its character representation.
     *
     * @param parEnumCellType The cell type to convert.
     * @return char The character representing the cell type.
     */
    static char toChar(EnumCellType parEnumCellType) {
        switch (parEnumCellType) {
            case EnumCellType::CELL:
                return ' ';
            case EnumCellType::CHECKER_BLACK:
                return 'b';
            case EnumCellType::CHECKER_WHITE:
                return 'w';
            case EnumCellType::CHECKER_BLACK_QUEEN:
                return 'B';
            case EnumCellType::CHECKER_WHITE_QUEEN:
                return 'W';
            default:
                return ' ';
        }
    }

    /**
     * @brief Converts a current cell enum value to its character representation.
     */
    [[nodiscard]] char toChar() const {
        return toChar(cellType_);
    }

private:
    /**
     *< Flag indicating if the cell is valid.
     */
    bool valid_ = false;
    /**
     *< The type of the cell.
     */
    EnumCellType cellType_ = EnumCellType::CELL;
};