/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*           Main Program File           */
/*     Developer: Sokolov Egor, 543M     */
/*          Version: 04.11.2025          */
/*****************************************/

#include <iostream>
#include <memory>
#include <string>
#include <random>
#include "logic/situation.h"
#include "logic/enum_cell_type.h"
#include "logic/depth_search_solver.h"
#include "minimax_solver.h"
#include "console_solver.h"
#include "method_type_enum.h"

/**
 * @brief Target evaluation function for board position.
 *
 * @param situation Situation to evaluate.
 */
int64_t evaluateScoreDepthSearch(const Situation& situation) {
    int64_t score = 0;
    if (!situation.isWhiteMove()) {
        score += situation.countBlackInitial() - situation.countBlack();
        score -= situation.countWhiteInitial() - situation.countWhite();
    } else {
        score += situation.countWhiteInitial() - situation.countWhite();
        score -= situation.countBlackInitial() - situation.countBlack();
    }
    return score;
}

/**
 * @brief Target evaluation function for board position.
 * Positive = better for WHITE (MAX), Negative = better for BLACK (MIN)
 *
 * @param situation Situation to evaluate.
 */
int64_t evaluateScoreMinimax(const Situation& situation) {
    int64_t score = 0;
    
    score += situation.countBlackInitial() - situation.countBlack();
    score -= situation.countWhiteInitial() - situation.countWhite();
    
    score += situation.countWhiteQueens() * 2;
    score -= situation.countBlackQueens() * 2;


    if (!situation.isWhiteMove()) {
        if (situation.countWhite() != 0 && !situation.canAnyCheckerMove(EnumCellType::CHECKER_WHITE)) {
            return -1000;
        }
    } else {
        if (situation.countBlack() != 0 && !situation.canAnyCheckerMove(EnumCellType::CHECKER_BLACK)) {
            return 1000;
        }
    }
    
    return score;
}


/**
 * @brief Target evaluation function for board position.
 *
 * @param situation Situation to evaluate.
 */
bool target(const Situation& situation) {
    if (!situation.isWhiteMove()) {
        return (situation.countBlack() == 0);
    } else {
        return (situation.countWhite() == 0);
    }
}

int main(int argc, char** argv) {
    std::unique_ptr<Situation> situation = std::make_unique<Situation>();
    size_t maxTreeDepth;
    MethodTypeEnum methodTypeEnum;

    std::cout <<"/*****************************************/\n"
                "/*            The basics of AI           */\n"
                "/*       Laboratory Works #4 #5 #6       */\n"
                "/*      Minimax; Alpha-beta pruning;     */\n"
                "/*               Statistics              */\n"
                "/*                -------                */\n"
                "/*     Developer: Sokolov Egor, 543M     */\n"
                "/*          Version: 06.12.2025          */\n"
                "/*****************************************/\n" << std::endl;

#if 0 // if it needs custom arrangement of figures
    EnumCellType checkerField[Situation::MAX_HEIGHT][Situation::MAX_WIDTH] = {};

    size_t fieldWidth;
    size_t fieldHeight;

    std::cin >> minimaxDepth;
    std::cin >> fieldHeight;
    std::cin >> fieldWidth;

    char tempCell;
    for (size_t i = 0; i < std::min(fieldHeight, (size_t)Situation::MAX_HEIGHT); i++) {
        for (size_t j = 0; j < std::min(fieldWidth, (size_t)Situation::MAX_WIDTH); j++) {
            std::cin >> tempCell;
            switch (tempCell) {
                case 'w':
                    checkerField[i][j] = EnumCellType::CHECKER_WHITE;
                    break;
                case 'W':
                    checkerField[i][j] = EnumCellType::CHECKER_WHITE_QUEEN;
                    break;
                case 'b':
                    checkerField[i][j] = EnumCellType::CHECKER_BLACK;
                    break;
                case 'B':
                    checkerField[i][j] = EnumCellType::CHECKER_BLACK_QUEEN;
                    break;
                case '-':
                default:
                    checkerField[i][j] = EnumCellType::CELL;
                    break;
            }
        }
    }
    situation = std::make_unique<Situation>(
            new Situation(checkerField, fieldWidth, fieldHeight));
#endif

    std::cout << "Input method number: " << std::endl;
    std::cout << "1. Depth search" << std::endl;
    std::cout << "2. Depth search with evaluation function" << std::endl;
    std::cout << "3. Minimax" << std::endl;
    std::cout << "4. Minimax with alpha-beta-pruning" << std::endl;
    size_t methodNumber;
    std::cin >> methodNumber;
    if ( methodNumber == 0 || methodNumber > 4) {
        std::cerr << "Wrong number of method!" << std::endl;
        return 1;
    }
    methodTypeEnum = static_cast<MethodTypeEnum>(methodNumber); //enum starts with 1

    std::cout << "Max tree depth = ";
    std::cin >> maxTreeDepth;

    std::unique_ptr<ISolver> solver;
    switch (methodTypeEnum) {
        case MethodTypeEnum::DFS:
            solver = std::make_unique<DepthSearchSolver>(maxTreeDepth);
            break;
        case MethodTypeEnum::DFS_WITH_EVALUATION_FUNC:
            solver = std::make_unique<DepthSearchSolver>(maxTreeDepth, evaluateScoreDepthSearch);
            break;
        case MethodTypeEnum::MINIMAX:
            solver = std::make_unique<MinimaxSolver>(maxTreeDepth, evaluateScoreMinimax, false);
            break;
        case MethodTypeEnum::MINIMAX_WITH_ALPHA_BETA_PRUNING:
            solver = std::make_unique<MinimaxSolver>(maxTreeDepth, evaluateScoreMinimax, true);
            break;
    }
    std::cout << std::endl;
    std::cout << "Initial situation:" << std::endl;
    situation->printField();

    ConsoleSolver consoleSolver;
    std::vector<Situation> foundPath = solver->solve(*situation, target);

    int64_t currentStep = 1;
    EnumGameCurrentStatus gameEndStatus = situation->currentGameStatus();
    do {
        std::cout << "\nStep " << currentStep << ": "
                  << (currentStep % 2 == 1 ? "White" : "Black") << " moves\n";

        if (foundPath.empty()) {
            break;
        }
        situation = std::make_unique<Situation>(foundPath[0]);
        currentStep++;

        situation->updateQueens();
        situation->printField();

        if (currentStep % 2 == 1) {
            foundPath = solver->solve(*situation, target);
        } else {
            while(situation->currentGameStatus() == EnumGameCurrentStatus::PLAYING) {
                foundPath = consoleSolver.solve(*situation, nullptr);
                if (!foundPath.empty()) {
                    break;
                }
                std::cout << "Wrong moving!\n\n";
            }
        }
        gameEndStatus = situation->currentGameStatus();
    } while (situation->currentGameStatus() == EnumGameCurrentStatus::PLAYING);
    std::cout << "\nThe end of the game: "
              << (gameEndStatus == EnumGameCurrentStatus::WHITE_VICTORY ? "White wins!" :
                  gameEndStatus == EnumGameCurrentStatus::BLACK_VICTORY ? "Black wins!" :
                  "Draw!") << "\n";
    return 0;
}
