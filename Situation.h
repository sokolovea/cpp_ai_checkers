/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*          CheckerField definition      */
/*      Developer: Sokolov Egor, 543M    */
/*           Version: 04.11.2025         */
/*****************************************/

#ifndef CHECKERS_CHECKERFIELD_H
#define CHECKERS_CHECKERFIELD_H
#include <iostream>
#include <string>
#include <vector>
#include "Cell.h"
#include "Coordinate.h"
#include "EnumCellType.h"
#include "EnumGameCurrentStatus.h"

/**
 * @brief Checker field with 8x8 cells
 */
class Situation;

/**
 * Evaluates the score of situation and helps to find the best next step.
 */
typedef int64_t (*EvaluateScore)(const Situation&);

/**
 * Evaluates target evaluation function for situation (is winner?).
 */
typedef bool(*Target)(const Situation&);

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
    int64_t currentChecherNumber_[2] = {0, 0};

    /**
     * The best situations vector for depthSearch with evaluate score function 
     */
    std::vector<Situation>* bestSituations_ = nullptr;

private:

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
     */
    int64_t limitFieldSize(int64_t parSize);

public:

    /**
     * @brief Constructor that initializes the game field with starting positions.
     */
    Situation(bool parShouldInitializeSituation = true);

    /**
     * @brief Constructor that initializes the game field with starting positions.
     */
    Situation(EnumCellType parField[Situation::MAX_HEIGHT][Situation::MAX_WIDTH], size_t parWidth, size_t parHeight);

    /**
     * @brief Copies all the fields except _bestSituations (installing to nullptr)
     */
    Situation(const Situation& other);

    /**
     * @brief Frees dynamically allocated fields
     */
    ~Situation();

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
     * @param checkerType The type of checker to check.
     * @return true if at least one checker can move, false otherwise.
     */
    [[nodiscard]] bool canAnyCheckerMove(EnumCellType checkerType) const;

    /**
     * @brief Updates checker pieces to queen status when they reach the opposite end.
     */
    void updateQueens();

    /**
     * @brief Returns the number of possible moves for the current player.
     *
     * This counts all valid moves (captures + simple moves) for the current player.
     */
    int64_t possibleMovesCount() const;

    /**
     * @brief Checks if a coordinate is valid.
     *
     * @param parCoordinate The coordinate to validate.
     * @return true if the coordinate is valid, false otherwise.
     */
    bool isCellValid(Coordinate parCoordinate) const;

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
    int64_t countWhiteInitial() const;

    /**
     * @brief Gets the initial count of black checkers.
     *
     * @return int64_t The initial number of black checkers.
     */
    int64_t countBlackInitial() const;

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
    bool isWhiteMove() const {
        return isWhiteMove_;
    }

    /**
     * @brief Generates next situation.
     */
    Situation generateNextSituation();

    /**
     * @brief Generates next best stituation scored by evaluate function.
     * 
     * @param parEvaluateScoreFunc The evaluate function.
     */
    Situation generateNextBestSituation(EvaluateScore parEvaluateScoreFunc);

    /**
    * @brief Generates next best stituation scored by evaluate function.
    */
    std::vector<Situation> generateAllNextSituations();

    /**
     * @brief Implements equals operator for situation's comparison 
     */
    bool operator==(const Situation& other) const;

};

#endif // CHECKERS_CHECKERFIELD_H
