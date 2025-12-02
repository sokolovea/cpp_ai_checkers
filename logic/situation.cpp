/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*       CheckerField implementation     */
/*      Developer: Sokolov Egor, 543M    */
/*           Version: 04.11.2025         */
/*****************************************/

#include <algorithm>
#include "situation.h"
#include "coordinate.h"
#include "enum_game_current_status.h"

/**
 * @brief Initializes the checkers field with starting positions.
 *
 * Sets up black checkers in the top three rows and white checkers
 * in the bottom three rows, with empty cells in between.
 */
Situation::Situation(bool parShouldInitializeSituation) {
    if (parShouldInitializeSituation) {
        for (int64_t i = 0; i < height_; i++) {
            for (int64_t j = 0; j < width_; j++) {
                if ((i + j) % 2 == 1) {
                    if (i < 3) {
                        field_[i][j] = EnumCellType::CHECKER_BLACK;
                        blackCheckersCount_++;
                    } else if (i > 4) {
                        field_[i][j] = EnumCellType::CHECKER_WHITE;
                        whiteCheckersCount_++;
                    } else {
                        field_[i][j] = EnumCellType::CELL;
                    }
                } else {
                    field_[i][j] = EnumCellType::CELL;
                }
            }
        }
        whiteCheckersCountInitial_ = whiteCheckersCount_;
        blackCheckersCountInitial_ = blackCheckersCount_;
    }
}

/**
 * @brief Constructor that initializes the game field with starting positions.
 */
Situation::Situation(EnumCellType parField[Situation::MAX_HEIGHT][Situation::MAX_WIDTH], size_t parWidth, size_t parHeight)
{
    width_ = limitFieldSize(parWidth);
    height_ = limitFieldSize(parHeight);

    for (int64_t i = 0; i < height_; i++) {
        for (int64_t j = 0; j < width_; j++) {
            switch (parField[i][j])
            {
            case EnumCellType::CHECKER_BLACK:
            case EnumCellType::CHECKER_BLACK_QUEEN:
                blackCheckersCount_++;
                break;
            case EnumCellType::CHECKER_WHITE:
            case EnumCellType::CHECKER_WHITE_QUEEN:
                whiteCheckersCount_++;
                break;
            default:
                break;
            }
            field_[i][j] = parField[i][j];
        }
    }
    updateQueens();
}

/**
 * @brief Copies all the fields except _bestSituations (installing to nullptr)
 */
Situation::Situation(const Situation& parOtherSituation) noexcept:
        width_(parOtherSituation.width_),
        height_(parOtherSituation.height_),
        whiteCheckersCount_(parOtherSituation.whiteCheckersCount_),
        blackCheckersCount_(parOtherSituation.blackCheckersCount_),
        whiteCheckersCountInitial_(parOtherSituation.whiteCheckersCountInitial_),
        blackCheckersCountInitial_(parOtherSituation.blackCheckersCountInitial_),
        isWhiteMove_(parOtherSituation.isWhiteMove_),
        bestSituations_(nullptr) {
    for (int64_t i = 0; i < MAX_HEIGHT; ++i) {
        for (int64_t j = 0; j < MAX_WIDTH; ++j) {
            field_[i][j] = parOtherSituation.field_[i][j];
        }
    }
    currentChecherNumber_[0] = parOtherSituation.currentChecherNumber_[0];
    currentChecherNumber_[1] = parOtherSituation.currentChecherNumber_[1];
}

/**
 * @brief Copy Assignment Operator
 */
Situation& Situation::operator=(const Situation & parOtherSituation) noexcept
{
    width_ = parOtherSituation.width_;
    height_ = parOtherSituation.height_;
    whiteCheckersCount_ = parOtherSituation.whiteCheckersCount_;
    blackCheckersCount_ = parOtherSituation.blackCheckersCount_;
    whiteCheckersCountInitial_ = parOtherSituation.whiteCheckersCountInitial_;
    blackCheckersCountInitial_ = parOtherSituation.blackCheckersCountInitial_;
    isWhiteMove_ = parOtherSituation.isWhiteMove_;
    bestSituations_ = nullptr;
    for (int64_t i = 0; i < MAX_HEIGHT; ++i) {
        for (int64_t j = 0; j < MAX_WIDTH; ++j) {
            field_[i][j] = parOtherSituation.field_[i][j];
        }
    }
    currentChecherNumber_[0] = parOtherSituation.currentChecherNumber_[0];
    currentChecherNumber_[1] = parOtherSituation.currentChecherNumber_[1];
    return *this;
}

/**
 * @brief Checks if a coordinate is valid within the field boundaries.
 *
 * @param parCoordinate The coordinate to validate.
 * @return true if the coordinate is within field boundaries, false otherwise.
 */
bool Situation::isCellValid(Coordinate parCoordinate) const {
    return (parCoordinate.row() < height_ && parCoordinate.col() < width_) &&
        (parCoordinate.row() >= 0 && parCoordinate.col() >= 0);
}

/**
 * @brief Gets the cell at the specified coordinate.
 *
 * @param parCoordinate The coordinate to query.
 * @return Cell object at the specified coordinate.
 */
Cell Situation::getCell(Coordinate parCoordinate) const {
    if (!isCellValid(parCoordinate)) {
        return {};
    }
    return {
        field_[parCoordinate.row()][parCoordinate.col()]
    };
}

/**
 * @brief Sets the cell type at the specified coordinate.
 *
 * @param parCoordinate The coordinate to modify.
 * @param parValue The cell type to set.
 */
void Situation::setCell(Coordinate parCoordinate, EnumCellType parValue) {
    if (isCellValid(parCoordinate)) {
        field_[parCoordinate.row()][parCoordinate.col()] = parValue;
    }
}

/**
 * @brief Limits the field size (width or height) before setting. 
 */
int64_t Situation::limitFieldSize(int64_t parSize) {
    if (parSize > 8 || parSize <= 0) {
        return 8;
    }
    return parSize;
}

/**
 * @brief Removes a checker from the specified coordinate.
 *
 * Decrements the appropriate checker count and sets the cell to empty.
 * @param parCoordinate The coordinate from which to remove the checker.
 */
void Situation::removeChecker(Coordinate parCoordinate) {
    if (isCellValid(parCoordinate)) {
        EnumCellType enumCellType = getCell(parCoordinate).cellType();
        if (CellFunc::isWhite(enumCellType)) {
            decrementWhite();
        } else if (CellFunc::isBlack(enumCellType)) {
            decrementBlack();
        }
        setCell(parCoordinate, EnumCellType::CELL);
    }
}

/**
 * @brief Moves a checker from one coordinate to another.
 *
 * @param parCoordinateFrom The source coordinate.
 * @param parCoordinateTo The destination coordinate.
 * @return true if the move was successful, false otherwise.
 */
bool Situation::moveChecker(Coordinate parCoordinateFrom, Coordinate parCoordinateTo) {
    if (isCellValid(parCoordinateFrom) && isCellValid(parCoordinateTo) &&
        !CellFunc::isChecker(getCell(parCoordinateTo).cellType())) {
            setCell(parCoordinateTo, getCell(parCoordinateFrom).cellType());
            setCell(parCoordinateFrom, EnumCellType::CELL);
            return true;
    }
    return false;
}

Situation::Situation(Situation && parOtherSituation) noexcept :
        width_(parOtherSituation.width_),
        height_(parOtherSituation.height_),
        whiteCheckersCount_(parOtherSituation.whiteCheckersCount_),
        blackCheckersCount_(parOtherSituation.blackCheckersCount_),
        whiteCheckersCountInitial_(parOtherSituation.whiteCheckersCountInitial_),
        blackCheckersCountInitial_(parOtherSituation.blackCheckersCountInitial_),
        isWhiteMove_(parOtherSituation.isWhiteMove_)
{
    for (int64_t i = 0; i < MAX_HEIGHT; ++i) {
        for (int64_t j = 0; j < MAX_WIDTH; ++j) {
            field_[i][j] = parOtherSituation.field_[i][j];
        }
    }
    currentChecherNumber_[0] = parOtherSituation.currentChecherNumber_[0];
    currentChecherNumber_[1] = parOtherSituation.currentChecherNumber_[1];
    bestSituations_ = std::move(parOtherSituation.bestSituations_);
}


Situation& Situation::operator=(Situation && parOtherSituation) noexcept
{
    width_ = parOtherSituation.width_;
    height_ = parOtherSituation.height_;
    whiteCheckersCount_ = parOtherSituation.whiteCheckersCount_;
    blackCheckersCount_ = parOtherSituation.blackCheckersCount_;
    whiteCheckersCountInitial_ = parOtherSituation.whiteCheckersCountInitial_;
    blackCheckersCountInitial_ = parOtherSituation.blackCheckersCountInitial_;
    isWhiteMove_ = parOtherSituation.isWhiteMove_;
    for (int64_t i = 0; i < MAX_HEIGHT; ++i) {
        for (int64_t j = 0; j < MAX_WIDTH; ++j) {
            field_[i][j] = parOtherSituation.field_[i][j];
        }
    }
    currentChecherNumber_[0] = parOtherSituation.currentChecherNumber_[0];
    currentChecherNumber_[1] = parOtherSituation.currentChecherNumber_[1];
    bestSituations_ = std::move(parOtherSituation.bestSituations_);
    return *this;
}

/**
 * @brief Prints current situation (checker field).
 */
void Situation::printField() const {
    std::cout << "    ";
    for (int64_t i = 0; i < width_; i++) {
        std::cout << (char)('A' + i) << "   "; 
    }
    std::cout << std::endl;
    std::cout << "  ";
    for (int64_t i = 0; i < width_; i++) {
        std::cout << "+---"; 
    }
    std::cout << std::endl;

    for (int64_t i = 0; i < height_; i++) {
        std::cout << (height_ - i) << " |";
        for (int64_t j = 0; j < width_; j++) {
            std::cout << " " << Cell::toChar(field_[i][j]) << " |";
        }
        std::cout << " " << (height_ - i) << "\n";
        std::cout << "  ";
        for (int64_t i = 0; i < width_; i++) {
            std::cout << "+---"; 
        }
        std::cout << std::endl;
    }

    std::cout << "    ";
    for (int64_t i = 0; i < width_; i++) {
        std::cout << (char)('A' + i) << "   "; 
    }
    std::cout << std::endl;
}

/**
 * @brief Checks if a capture move is possible.
 *
 * @param parCoordinateFrom The source coordinate.
 * @param parCoordinateTo The destination coordinate.
 * @param parIsQueen Flag indicating if the piece is a queen.
 * @return true if a capture is possible, false otherwise.
 */
bool Situation::canCapture(Coordinate parCoordinateFrom, Coordinate parCoordinateTo, bool parIsQueen) const {
    if (!isCellValid(parCoordinateFrom) || !isCellValid(parCoordinateTo)) {
        return false;
    }

    if (field_[parCoordinateTo.row()][parCoordinateTo.col()] != EnumCellType::CELL) {
        return false;
    }

    int64_t deltaRow = parCoordinateTo.row() - parCoordinateFrom.row();
    int64_t deltaCol = parCoordinateTo.col() - parCoordinateFrom.col();

    // diagonal only!
    if (std::abs(deltaRow) != std::abs(deltaCol)) {
        return false;
    }

    int64_t stepRow = (deltaRow > 0) ? 1 : -1;
    int64_t stepCol = (deltaCol > 0) ? 1 : -1;

    int64_t enemyCount = 0;
    int64_t row = parCoordinateFrom.row() + stepRow;
    int64_t col = parCoordinateFrom.col() + stepCol;

    while (row != parCoordinateTo.row() && col != parCoordinateTo.col()) {
        EnumCellType currentCellType = field_[row][col];

        if (CellFunc::isChecker(currentCellType)) {
            if (!CellFunc::areDifferentColorsCheckers(
                field_[parCoordinateFrom.row()][parCoordinateFrom.col()],
                currentCellType)) {
                return false; // Своя шашка на пути
            }
            enemyCount++;

            // Для обычной шашки после вражеской должна сразу следовать конечная клетка
            if (!parIsQueen) {
                if (enemyCount > 1 || (row + stepRow != parCoordinateTo.row())) {
                    return false;
                }
            }
        } else if (currentCellType != EnumCellType::CELL) {
            return false; // Непустая клетка, не являющаяся шашкой
        }

        row += stepRow;
        col += stepCol;
    }

    if (parIsQueen) {
        return enemyCount == 1; // Дамка должна взять ровно одну шашку
    } else {
        return enemyCount == 1 && std::abs(deltaRow) == 2; // Обычная шашка на расстояние 2
    }
}

/**
 * @brief Executes a capture move.
 *
 * @param parCoordinateFrom The source coordinate.
 * @param parCoordinateTo The destination coordinate.
 * @return true if the capture was successful, false otherwise.
 */
bool Situation::capture(Coordinate parCoordinateFrom, Coordinate parCoordinateTo) {
    if (!isCellValid(parCoordinateFrom) || !isCellValid(parCoordinateTo)) {
        return false;
    }

    EnumCellType movingChecker = field_[parCoordinateFrom.row()][parCoordinateFrom.col()];
    bool isQueen = CellFunc::isQueen(movingChecker);

    if (!canCapture(parCoordinateFrom, parCoordinateTo, isQueen)) {
        return false;
    }

    int64_t deltaRow = parCoordinateTo.row() - parCoordinateFrom.row();
    int64_t deltaCol = parCoordinateTo.col() - parCoordinateFrom.col();
    int64_t stepRow = (deltaRow > 0) ? 1 : -1;
    int64_t stepCol = (deltaCol > 0) ? 1 : -1;

    int64_t row = parCoordinateFrom.row() + stepRow;
    int64_t col = parCoordinateFrom.col() + stepCol;
    Coordinate capturedCheckerCoord(-1, -1);

    while (row != parCoordinateTo.row() && col != parCoordinateTo.col()) {
        if (CellFunc::isChecker(field_[row][col]) &&
            CellFunc::areDifferentColorsCheckers(movingChecker, field_[row][col])) {
            capturedCheckerCoord = Coordinate(row, col);
            break;
            }
        row += stepRow;
        col += stepCol;
    }

    if (!isCellValid(capturedCheckerCoord)) {
        return false;
    }

    removeChecker(capturedCheckerCoord);

    setCell(parCoordinateTo, movingChecker);
    setCell(parCoordinateFrom, EnumCellType::CELL);

    if (!isQueen) {
        if ((CellFunc::isWhite(movingChecker) && parCoordinateTo.row() == 0) ||
            (CellFunc::isBlack(movingChecker) && parCoordinateTo.row() == height_ - 1)) {
            setCell(parCoordinateTo, CellFunc::promoteToQueen(movingChecker));
            }
    }
    return true;
}

/**
 * @brief Gets the count of white checkers.
 *
 * @return int64_t The number of white checkers.
 */
int64_t Situation::countWhite() const {
    return whiteCheckersCount_;
}

/**
 * @brief Decrements the count of white checkers.
 */
void Situation::decrementWhite() {
    if (whiteCheckersCount_ > 0) {
        whiteCheckersCount_--;
    }
}

/**
 * @brief Decrements the count of black checkers.
 */
void Situation::decrementBlack() {
    if (blackCheckersCount_ > 0) {
        blackCheckersCount_--;
    }
}

/**
 * @brief Gets the count of black checkers.
 *
 * @return int64_t The number of black checkers.
 */
int64_t Situation::countBlack() const {
    return blackCheckersCount_;
}

/**
 * @brief Gets the count of white queens on the field.
 *
 * @return int64_t The number of white queens.
 */
int64_t Situation::countWhiteQueens() const
{
    int64_t count = 0;
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            if (field_[i][j] == EnumCellType::CHECKER_WHITE_QUEEN) {
                count++;
            }
        }
    }
    return count;
}

/**
 * @brief Gets the count of black queens on the field.
 *
 * @return int64_t The number of black queens.
 */
int64_t Situation::countBlackQueens() const
{
    int64_t count = 0;
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            if (field_[i][j] == EnumCellType::CHECKER_BLACK_QUEEN) {
                count++;
            }
        }
    }
    return count;
}

/**
 * @brief Gets the initial count of black checkers.
 *
 * @return int64_t The initial number of black checkers.
 */
int64_t Situation::countWhiteInitial() const {
    return blackCheckersCountInitial_;
}

/**
 * @brief Gets the initial count of black checkers.
 *
 * @return int64_t The initial number of black checkers.
 */
int64_t Situation::countBlackInitial() const {
    return whiteCheckersCountInitial_;
}

/**
 * @brief Determines the current game status.
 *
 * @return EnumGameCurrentStatus The current status of the game.
 */
EnumGameCurrentStatus Situation::currentGameStatus() const {
    if (countBlack() == 0) {
        return EnumGameCurrentStatus::WHITE_VICTORY;
    }
    if (countWhite() == 0) {
        return EnumGameCurrentStatus::BLACK_VICTORY;
    }

    // Draw additional
    bool blackCanMove = canAnyCheckerMove(EnumCellType::CHECKER_BLACK);
    bool whiteCanMove = canAnyCheckerMove(EnumCellType::CHECKER_WHITE);

    if (!blackCanMove && !whiteCanMove) {
        return EnumGameCurrentStatus::DRAW;
    }

    return EnumGameCurrentStatus::PLAYING;
}

/**
 * @brief Checks if any checker of the specified type can move.
 *
 * @param checkerType The type of checker to check.
 * @return true if at least one checker can move, false otherwise.
 */
bool Situation::canAnyCheckerMove(EnumCellType checkerType) const {
    bool isWhite = CellFunc::isWhite(checkerType);
    
    for (int64_t i = (isWhite ? height_ - 1 : 0);
         (isWhite ? i >= 0 : i < height_);
         (isWhite ? i-- : i++)) {
        for (int64_t j = 0; j < width_; j++) {
            Coordinate from(i, j);
            EnumCellType cellType = field_[i][j];
            
            if ((isWhite && CellFunc::isWhite(cellType)) ||
                (!isWhite && CellFunc::isBlack(cellType))) {
                
                bool isQueen = CellFunc::isQueen(cellType);
                
                // Check all diagonal directions
                for (int64_t di = -1; di <= 1; di += 2) {
                    for (int64_t dj = -1; dj <= 1; dj += 2) {
                        // For regular checkers - check only forward directions
                        if (!isQueen) {
                            if (isWhite && di > 0) continue; // White regular checkers can't move down
                            if (!isWhite && di < 0) continue; // Black regular checkers can't move up
                        }
                        
                        // Check regular move (1 cell)
                        Coordinate to(i + di, j + dj);
                        if (isCellValid(to) && 
                            field_[to.row()][to.col()] == EnumCellType::CELL) {
                            return true;
                        }
                        
                        // Check capture move (2 cells)
                        Coordinate captureTo(i + 2 * di, j + 2 * dj);
                        if (isCellValid(captureTo) && 
                            field_[captureTo.row()][captureTo.col()] == EnumCellType::CELL) {
                            
                            Coordinate middle(i + di, j + dj);
                            if (isCellValid(middle)) {
                                EnumCellType middleCell = field_[middle.row()][middle.col()];
                                // Check if middle cell contains opponent's checker
                                if (middleCell != EnumCellType::CELL && 
                                    ((isWhite && CellFunc::isBlack(middleCell)) || 
                                     (!isWhite && CellFunc::isWhite(middleCell)))) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

/**
 * @brief Updates checkers to queen status when they reach the opposite end.
 */
void Situation::updateQueens() {
    for (int64_t j = 0; j < width_; j++) {
        if (field_[0][j] == EnumCellType::CHECKER_WHITE) {
            field_[0][j] = EnumCellType::CHECKER_WHITE_QUEEN;
        }
    }

    for (int64_t j = 0; j < width_; j++) {
        if (field_[height_ - 1][j] == EnumCellType::CHECKER_BLACK) {
            field_[height_ - 1][j] = EnumCellType::CHECKER_BLACK_QUEEN;
        }
    }
}


/**
 * @brief Returns the number of possible moves for the current player.
 *
 * This counts all valid moves (captures + simple moves) for the current player.
 */
int64_t Situation::possibleMovesCount() const {
    int64_t count = 0;
    bool isWhite = isWhiteMove_;

    for (int64_t i = (isWhite ? height_ - 1 : 0);
         (isWhite ? i >= 0 : i < height_);
         (isWhite ? i-- : i++)) {
        for (int64_t j = 0; j < width_; j++) {
            Coordinate from(i, j);
            EnumCellType cellType = field_[i][j];

            if ((isWhite && CellFunc::isWhite(cellType)) ||
                (!isWhite && CellFunc::isBlack(cellType))) {
                
                bool isQueen = CellFunc::isQueen(cellType);

                for (int64_t di = -2; di <= 2; di++) {
                    for (int64_t dj = -2; dj <= 2; dj++) {
                        if (di == 0 || dj == 0 || std::abs(di) != std::abs(dj)) continue;

                        // Regular checker: only forward
                        if (!isQueen) {
                            if (isWhite && di > 0) continue;
                            if (!isWhite && di < 0) continue;
                        }

                        Coordinate to(i + di, j + dj);
                        if (!isCellValid(to)) continue;

                        if (canCapture(from, to, isQueen) ||
                            (std::abs(di) == 1 && std::abs(dj) == 1 &&
                             field_[to.row()][to.col()] == EnumCellType::CELL)) {
                            count++;
                        }
                    }
                }
            }
        }
    }

    return count;
}

/**
 * @brief Generates next situation.
 */
Situation Situation::generateNextSituation() {
    std::pair<Coordinate, Coordinate> coordinatesForNextSituation;
    int64_t& nextSituationCounter = currentChecherNumber_[isWhiteMove_ ? 0 : 1];
    int64_t currentSituationCounter = 0;
    bool isSituationFound = false;

    for (int64_t i = (isWhiteMove_ ? height_ - 1 : 0);
        (isWhiteMove_ ? i >= 0 : i < height_);
        (isWhiteMove_ ? i-- : i++)) {
        for (int64_t j = 0; j < width_; j++) {
            Coordinate from(i, j);
            auto type = getCell(from).cellType();

            if ((isWhiteMove_ && CellFunc::isWhite(type)) ||
                (!isWhiteMove_ && CellFunc::isBlack(type))) {
                
                bool isQueen = CellFunc::isQueen(type);
                for (int di = -2; di <= 2; di++) {
                    for (int dj = -2; dj <= 2; dj++) {
                        if (di == 0 || dj == 0 || abs(di) != abs(dj)) continue;

                        if (!isQueen) {
                            if (CellFunc::isWhite(type) && di > 0) continue;
                            if (CellFunc::isBlack(type) && di < 0) continue;
                        }

                        Coordinate to(i + di, j + dj);
                        if (!isCellValid(to)) continue;

                        if (canCapture(from, to, isQueen) ||
                            (abs(di) == 1 && abs(dj) == 1 &&
                            !CellFunc::isChecker(getCell(to).cellType()))) {
                            if (currentSituationCounter == nextSituationCounter) {
                                coordinatesForNextSituation = {from, to};
                                nextSituationCounter++;
                                isSituationFound = true;
                                goto exit_loops;
                            }
                            currentSituationCounter++;
                        }
                    }
                }
            }
        }
    }

exit_loops:
    if (!isSituationFound) {
        currentChecherNumber_[isWhiteMove_ ? 0 : 1] = 0;
        return *this;
    }

    Situation nextSituation(*this);
    auto [from, to] = coordinatesForNextSituation;
    bool isQueen = CellFunc::isQueen(getCell(from).cellType());

    if (canCapture(from, to, isQueen))
        nextSituation.capture(from, to);
    else
        nextSituation.moveChecker(from, to);

    nextSituation.updateQueens();
    nextSituation.isWhiteMove_ = !nextSituation.isWhiteMove_;
    return nextSituation;
}

/**
 * @brief 
 */
bool Situation::applyMoveIfPossible(Coordinate parFirst, Coordinate parSecond) {
    bool isMoveCorrect = false;
    if (isCellValid(parFirst)) {
        if (isWhiteMove()) {
            isMoveCorrect = (getCell(parFirst).cellType() == EnumCellType::CHECKER_WHITE || 
                getCell(parFirst).cellType() == EnumCellType::CHECKER_WHITE_QUEEN);
        } else {
            isMoveCorrect = (getCell(parFirst).cellType() == EnumCellType::CHECKER_BLACK || 
                getCell(parFirst).cellType() == EnumCellType::CHECKER_BLACK_QUEEN);  
        }
    }
    if (isMoveCorrect) {
        isMoveCorrect = moveChecker(parFirst, parSecond);
        if (isMoveCorrect) {
            isWhiteMove_ = !isWhiteMove_;
        }
    }
    return isMoveCorrect;
}

/**
 * @brief Generates next best stituation scored by evaluate function.
* 
* @param parEvaluateScoreFunc The evaluate function.
*/
Situation Situation::generateNextBestSituation(EvaluateScore parEvaluateScoreFunc) {
    if (bestSituations_ == nullptr) {
        bestSituations_ = std::make_unique<std::vector<Situation>>();
        size_t situationsCount = possibleMovesCount();
        for (size_t i = 0; i < situationsCount; i++) {
            bestSituations_->push_back(generateNextSituation());
        }

        std::sort(bestSituations_->begin(), bestSituations_->end(), 
                [parEvaluateScoreFunc](const Situation &left, const Situation &right) {
            return parEvaluateScoreFunc(left) < parEvaluateScoreFunc(right);
        });    
    }
    if (bestSituations_->size() != 0) {
        Situation best = (*bestSituations_)[bestSituations_->size() - 1];
        bestSituations_->pop_back();
        return best;
    }
    return Situation(*this);
}


/**
 * @brief Generates all next situations.
*/
std::vector<Situation> Situation::generateAllNextSituations() {
    std::vector<Situation> nextSituations = std::vector<Situation>();
    size_t situationsCount = possibleMovesCount();
    for (size_t i = 0; i < situationsCount; i++) {
        nextSituations.push_back(generateNextSituation());
    }   
    return nextSituations;
}

/**
 * Implements equals operator for situation's comparison 
 */
bool Situation::operator==(const Situation& other) const {
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            if (field_[i][j] != other.field_[i][j]) {
                return false;
            }
        }
    }
    return true;
}

