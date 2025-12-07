/*****************************************/
/*           Laboratory Work #6          */
/*            The basics of AI           */
/*           Main Program File           */
/*     Developer: Sokolov Egor, 543M     */
/*          Version: 04.11.2025          */
/*****************************************/

#include <iostream>
#include <memory>

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
int64_t evaluateScoreDepthSearchFunc(const Situation &situation) {
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
int64_t evaluateScoreMinimaxFunc(const Situation &situation) {
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
bool targetFunc(const Situation &situation) {
    if (!situation.isWhiteMove()) {
        return (situation.countBlack() == 0);
    } else {
        return (situation.countWhite() == 0);
    }
}

int main() {
    std::unique_ptr<Situation> situation = std::make_unique<Situation>();
    size_t maxTreeDepth;
    MethodTypeEnum methodTypeEnum;

    std::cout << "/*****************************************/\n"
                 "/*            The basics of AI           */\n"
                 "/*           Laboratory Works #6         */\n"
                 "/*               Statistics              */\n"
                 "/*                -------                */\n"
                 "/*     Developer: Sokolov Egor, 543M     */\n"
                 "/*          Version: 07.12.2025          */\n"
                 "/*****************************************/\n" << std::endl;

    std::cout << "Input method number: " << std::endl;
    std::cout << "1. Depth search" << std::endl;
    std::cout << "2. Depth search with evaluation function" << std::endl;
    std::cout << "3. Minimax" << std::endl;
    std::cout << "4. Minimax with alpha-beta-pruning" << std::endl;
    size_t methodNumber;
    std::cin >> methodNumber;
    if (methodNumber == 0 || methodNumber > 4) {
        std::cerr << "Wrong number of method!" << std::endl;
        return 1;
    }
    methodTypeEnum = static_cast<MethodTypeEnum>(methodNumber); //enum starts with 1

    std::cout << "Max tree depth = ";
    std::cin >> maxTreeDepth;

    std::unique_ptr<ISolver> solver;
    switch (methodTypeEnum) {
        case MethodTypeEnum::DFS:
            solver = std::make_unique<DepthSearchSolver>(maxTreeDepth, targetFunc);
            break;
        case MethodTypeEnum::DFS_WITH_EVALUATION_FUNC:
            solver = std::make_unique<DepthSearchSolver>(maxTreeDepth, targetFunc, evaluateScoreDepthSearchFunc);
            break;
        case MethodTypeEnum::MINIMAX:
            solver = std::make_unique<MinimaxSolver>(maxTreeDepth, targetFunc, evaluateScoreMinimaxFunc, false);
            break;
        case MethodTypeEnum::ALPHA_BETA_PRUNING:
            solver = std::make_unique<MinimaxSolver>(maxTreeDepth, targetFunc, evaluateScoreMinimaxFunc, true);
            break;
    }
    std::cout << std::endl;
    std::cout << "Initial situation:" << std::endl;
    situation->printField();

    ConsoleSolver consoleSolver;
    std::vector<Situation> foundPath = solver->solve(*situation);

    int64_t currentStep = 1;

    double d = 0;
    double l = 0;
    double n = 0;

    d += static_cast<double>(maxTreeDepth);
    l += static_cast<double>(foundPath.size());
    n += static_cast<double>(ISolver::getN());

    static double stat_counter = 1;

    std::cout << "\nSTATISTICS:" << std::endl;
    std::cout << "Average D = " << d / stat_counter << std::endl;
    std::cout << "Average L = " << l / stat_counter << std::endl;
    std::cout << "Average N = " << n / stat_counter << std::endl << std::endl;

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
            foundPath = solver->solve(*situation);

            ++stat_counter;

            d += static_cast<double>(maxTreeDepth);
            l += static_cast<double>(foundPath.size());
            n = static_cast<double>(ISolver::getN());

            std::cout << "\nSTATISTICS:" << std::endl;
            std::cout << "Average D = " << d / stat_counter << std::endl;
            std::cout << "Average L = " << l / stat_counter << std::endl;
            std::cout << "Average N = " << n / stat_counter << std::endl << std::endl;

        } else {
            while (situation->currentGameStatus() == EnumGameCurrentStatus::PLAYING) {
                foundPath = consoleSolver.solve(*situation);
                if (!foundPath.empty()) {
                    break;
                }
                std::cout << "Wrong moving!\n\n";
            }
        }
        gameEndStatus = situation->currentGameStatus();
    } while (situation->currentGameStatus() == EnumGameCurrentStatus::PLAYING);
    std::cout << "\nThe end of the game: "
            << (gameEndStatus == EnumGameCurrentStatus::WHITE_VICTORY
                    ? "White wins!"
                    : gameEndStatus == EnumGameCurrentStatus::BLACK_VICTORY
                          ? "Black wins!"
                          : "Draw!") << "\n";
    return 0;
}
