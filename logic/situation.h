/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*          CheckerField definition      */
/*      Developer: Sokolov Egor, 543M    */
/*           Version: 04.11.2025         */
/*****************************************/

#pragma once

#include <vector>
#include <memory>
#include "cell.h"
#include "coordinate.h"
#include "enum_cell_type.h"
#include "enum_game_current_status.h"

/**
 * @brief Checker field with 8x8 cells
 */
class Situation;

/**
 * Target function that evaluates the situation and returns current score of player.
 */
typedef bool (*Target)(const Situation&);

/**
 * Evaluates the score of situation and helps to find the best next step.
 */
typedef int64_t (*EvaluateScore)(const Situation&);

/**
 * @brief Checker player enum
 */
enum class CheckerPlayer {
    /** White player */
    WHITE,
    /** Black player */
    BLACK,
};

/**
 * @brief Checker field with 8x8 cells
 */
class Situation {
public:
    /**
     * The maximum width of the field
     */
    static constexpr int64_t MAX_WIDTH = 8;
    /**
    * The maimum height of the field
    */
    static constexpr int64_t MAX_HEIGHT = 8;

private:
    /**
     * The current width of the field
     */
    int64_t width_ = MAX_WIDTH;
    /**
    * The current height of the field
    */
    int64_t height_ = MAX_HEIGHT;
    /**
    * The count of white checkers on the field
    */
    int64_t whiteCheckersCount_ = 0;
    /**
    * The count of black checkers on the field
    */
    int64_t blackCheckersCount_ = 0;
    /**
    * The initial count of white checkers on the field
    */
    int64_t whiteCheckersCountInitial_ = 0;
    /**
    * The initial count of black checkers on the field
    */
    int64_t blackCheckersCountInitial_ = 0;
    /**
     * The field with checkers
     */
    EnumCellType field_[MAX_HEIGHT][MAX_WIDTH] = {};
    /**
     * The current player is on white or black opposite
     */
    bool isWhiteMove_ = true;
    /**
     * The current cheker number array ()
     */
    int64_t currentCheckerNumber_[2] = {0, 0};

    /**
     * The best situations vector for depthSearch with evaluate score function 
     */
    std::unique_ptr<std::vector<Situation>> bestSituations_;

    /**
     * @brief Decrements the count of white checkers.
     */
    void decrementWhite();

    /**
    * @brief Decrement count of black cells
    */
    void decrementBlack();

    /**
     * @brief Sets the cell content at the specified coordinate.
     *
     * @param parCoordinate The coordinate to modify.
     * @param parValue The value to set at the coordinate.
     */
    void setCell(Coordinate parCoordinate, EnumCellType parValue);

    /**
     * @brief Limits the field size (width or height) before setting.
     *
     * @param parSize parameter to be limited
     */
    static int64_t limitFieldSize(int64_t parSize);

public:

    /**
     * @brief Constructs an empty or standard initial checkers position.
     * @param parShouldInitializeSituation If true (default), places black checkers on rows 0–2 and white on rows 5–7.
     */
    explicit Situation(bool parShouldInitializeSituation = true);

    /**
     * @brief Constructs a situation from a raw field array with custom dimensions.
     * @param parField  Object containing the initial board state.
     * @param parWidth  Desired board width.
     * @param parHeight Desired board height.
     */
    explicit Situation(EnumCellType parField[MAX_HEIGHT][MAX_WIDTH], size_t parWidth, size_t parHeight);

    /**
     * @brief Copy constructor. Performs deep copy of the board; bestSituations_ is set to nullptr.
     * @param parOtherSituation Situation to copy from.
     */
    Situation(const Situation& parOtherSituation) noexcept;

    /**
     * @brief Copy assignment operator. Deep-copies the board; bestSituations_ is cleared.
     * @param parOtherSituation Situation to copy from.
     * @return Reference to this object.
     */
    Situation& operator=(const Situation& parOtherSituation) noexcept;

    /**
     * @brief Move constructor. Transfers ownership of all data including bestSituations_.
     * @param parOtherSituation Situation to move from.
     */
    Situation(Situation&& parOtherSituation) noexcept;

    /**
     * @brief Move assignment operator.
     * @param parOtherSituation Situation to move from.
     * @return Reference to this object.
     */
    Situation& operator=(Situation&& parOtherSituation) noexcept;

    /**
     * @brief Default destructor
     */
    ~Situation() noexcept = default;

    /**
     * @brief Prints the current state of the game field to the console.
     */
    void printField() const;

    /**
     * @brief Gets the current game status.
     *
     * @return EnumGameCurrentStatus The current status of the game.
     */
    [[nodiscard]] EnumGameCurrentStatus currentGameStatus() const;

    /**
     * @brief Checks if any checker of the specified type can make a move.
     *
     * @param parCheckerType The type of checker to check.
     * @return true if at least one checker can move, false otherwise.
     */
    [[nodiscard]] bool canAnyCheckerMove(EnumCellType parCheckerType) const;

    /**
     * @brief Updates checker pieces to queen status when they reach the opposite end.
     */
    void updateQueens();

    /**
     * @brief Returns the number of possible moves for the current player.
     *
     * This counts all valid moves (captures + simple moves) for the current player.
     */
    [[nodiscard]] int64_t possibleMovesCount() const;

    /**
     * @brief Checks if a coordinate is valid.
     *
     * @param parCoordinate The coordinate to validate.
     * @return true if the coordinate is valid, false otherwise.
     */
    [[nodiscard]] bool isCellValid(Coordinate parCoordinate) const;

    /**
     * @brief Removes a checker from the specified coordinate.
     *
     * @param parCoordinate The coordinate from which to remove the checker.
     */
    void removeChecker(Coordinate parCoordinate);

    /**
     * @brief Moves a checker from one coordinate to another.
     *
     * @param parCoordinateFrom The source coordinate.
     * @param parCoordinateTo The destination coordinate.
     * @return true if the move was successful, false otherwise.
     */
    bool moveChecker(Coordinate parCoordinateFrom, Coordinate parCoordinateTo);

    /**
     * @brief Checks if a capture move is possible.
     *
     * @param parCoordinateFrom The source coordinate.
     * @param parCoordinateTo The destination coordinate.
     * @param parIsQueen Flag indicating if the piece is a queen.
     * @return true if a capture is possible, false otherwise.
     */
    [[nodiscard]] bool canCapture(Coordinate parCoordinateFrom, Coordinate parCoordinateTo, bool parIsQueen) const;

    /**
     * @brief Executes a capture move.
     *
     * @param parCoordinateFrom The source coordinate.
     * @param parCoordinateTo The destination coordinate.
     * @return true if the capture was successful, false otherwise.
     */
    bool capture(Coordinate parCoordinateFrom, Coordinate parCoordinateTo);

    /**
     * @brief Gets the count of white checkers on the field.
     *
     * @return int64_t The number of white checkers.
     */
    [[nodiscard]] int64_t countWhite() const;

    /**
     * @brief Gets the count of black checkers on the field.
     *
     * @return int64_t The number of black checkers.
     */
    [[nodiscard]] int64_t countBlack() const;

    /**
     * @brief Gets the count of white queens on the field.
     *
     * @return int64_t The number of white queens.
     */
    [[nodiscard]] int64_t countWhiteQueens() const;

    /**
     * @brief Gets the count of black queens on the field.
     *
     * @return int64_t The number of black queens.
     */
    [[nodiscard]] int64_t countBlackQueens() const;

    /**
     * @brief Gets the initial count of black checkers.
     *
     * @return int64_t The initial number of black checkers.
     */
    [[nodiscard]] int64_t countWhiteInitial() const;

    /**
     * @brief Gets the initial count of black checkers.
     *
     * @return int64_t The initial number of black checkers.
     */
    [[nodiscard]] int64_t countBlackInitial() const;

    /**
     * @brief Gets the cell content at the specified coordinate.
     *
     * @param parCoordinate The coordinate to query.
     * @return Cell The cell at the specified coordinate.
     */
    [[nodiscard]] Cell getCell(Coordinate parCoordinate) const;

    /**
     * @brief The current player is on white or black opposite
     */
    [[nodiscard]] bool isWhiteMove() const {
        return isWhiteMove_;
    }

    /**
     * @brief Generates next situation.
     *
     * @return Next situation (if not possible, return *this)
     */
    Situation generateNextSituation();

    /**
     * @brief Applies a move if it belongs to the current player and is legal.
     *
     * @param parFirst  Source coordinate.
     * @param parSecond Destination coordinate.
     * @return true if the move was applied, false otherwise.
     */
    bool applyMoveIfPossible(Coordinate parFirst, Coordinate parSecond);

    /**
     * @brief Generates next best situation scored by evaluate function.
     * 
     * @param parEvaluateScoreFunc The evaluate function.
     */
    Situation generateNextBestSituation(EvaluateScore parEvaluateScoreFunc);

    /**
    * @brief Generates next best stituation scored by evaluate function.
    *
    * @return Vector of all successor situations.
    */
    std::vector<Situation> generateAllNextSituations();

    /**
     * @brief Equality operator – compares only the board layout (not metadata like counters).
     *
     * @param parOther Situation to compare with.
     * @return true if boards are identical.
     */
    bool operator==(const Situation& parOther) const;

};

