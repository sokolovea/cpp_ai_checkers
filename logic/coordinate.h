/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*       Coordinate implementation       */
/*      Developer: Sokolov Egor, 543M    */
/*           Version: 04.11.2025         */
/*****************************************/

#pragma once

/**
 * @brief Represents a coordinate on the checkers board.
 */
class Coordinate {
public:
    /**
     * @brief Gets the row coordinate.
     */
    [[nodiscard]] int64_t row() const {
        return row_;
    }

    /**
     * @brief Sets the row coordinate.
     *
     * @param parRow The row index to set.
     */
    void setRow(int64_t parRow) {
        row_ = parRow;
    }

    /**
     * @brief Gets the column coordinate.
     */
    [[nodiscard]] int64_t col() const {
        return _col;
    }

    /**
     * @brief Sets the column coordinate.
     *
     * @param parCol The column character to set.
     */
    void setCol(int64_t parCol) {
        _col = parCol;
    }

    /**
     * @brief Constructs a new default Coordinate object.
     */
    Coordinate()
        : row_(-1),
          _col(-1) {
    }

    /**
     * @brief Constructs a new Coordinate object.
     *
     * @param parRow The row index.
     * @param parCol The column character.
     */
    Coordinate(int64_t parRow, int64_t parCol)
        : row_(parRow),
          _col(parCol) {
    }

private:
    /**
     * The row index of the coordinate.
     */
    int64_t row_;
    /**
     * The column character of the coordinate.
     */
    int64_t _col;
};

/**
 * @brief Comparison operator for Coordinate objects.
 *
 * @param parFirst The left-hand side Coordinate.
 * @param parSecond The right-hand side Coordinate.
 * @return true if coordinates are equal, false otherwise.
 */
inline bool operator==(const Coordinate& parFirst, const Coordinate& parSecond) {
    return (parFirst.row() == parSecond.row()) &&
           (parFirst.col() == parSecond.col());
}
