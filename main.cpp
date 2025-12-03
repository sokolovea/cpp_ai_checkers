/*****************************************/
/*           Laboratory Work #4          */
/*            The basics of AI           */
/*           Main Program File           */
/*     Developer: Sokolov Egor, 543M     */
/*          Version: 04.11.2025          */
/*****************************************/

#include <iostream>
#include <string>
#include <random>
#include "logic/situation.h"
#include "logic/enum_cell_type.h"
#include "logic/ai_solver.h"

/**
 * @brief Target evaluation function for board position.
 *
 * @param situation Situation to evaluate.
 */
int64_t evaluateScore(const Situation& situation) {
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

    if (situation.isWhiteMove()) {
        score -= situation.countWhite() / 2;
    }
    if ( situation.isWhiteMove()) {
        score += situation.countBlack() / 2;
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
    Situation* situation;
    size_t minimaxDepth;

    std::cout <<"/*****************************************/\n"
                "/*            The basics of AI           */\n"           
                "/*       Laboratory Works #4 and #5      */\n"
                "/*     Minimax and Alpha-beta pruning    */\n"
                "/*                -------                */\n"
                "/*     Developer: Sokolov Egor, 543M     */\n"
                "/*          Version: 13.11.2025          */\n"
                "/*****************************************/\n" << std::endl;

    if (argc < 2) {
        situation = new Situation();
    } else {
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
        situation = new Situation(checkerField, fieldWidth, fieldHeight);
    }


    std::cout << "Max depth for min-max tree = ";
    std::cin >> minimaxDepth;
    if (minimaxDepth <= 0) {
        std::cout << "Depth must be > 0!" << std::endl;
        return 1;
    }
    std::cout << std::endl;

    std::cout << "Initial situation:" << std::endl;
    situation->printField();

    // std::vector<Situation> foundPath = AiSolver::solvedepthSearch(*situation, target, 500 /* depthSearchMaxDepth */);
    // std::vector<Situation> foundPath = AiSolver::solveDepthSearchWithScoreFunc(*situation, target, 100, evaluateScore);
    // std::vector<Situation> foundPath = AiSolver::solveMinimax(*situation, target, minimaxDepth, evaluateScoreMinimax);
    // std::vector<Situation> foundPath = AiSolver::solveMinimaxAlphaBeta(*situation, target, minimaxDepth, evaluateScoreMinimax);
    std::vector<Situation> foundPath = AiSolver::solveMinimaxAlphaBetaOptimized(*situation, target, minimaxDepth, evaluateScoreMinimax);

    // if (foundPath.size() == 0) {
    //     std::cout << "No possible moves!\n";
    //     return 0;
    // }
    int64_t currentStep = 1;
    EnumGameCurrentStatus gameEndStatus = situation->currentGameStatus();
    do {
        std::cout << "\nStep " << currentStep << ": "
                  << (currentStep % 2 == 1 ? "White" : "Black") << " moves\n";

        if (foundPath.size() == 0) {
            break;
        }
        delete situation;
        situation = new Situation(foundPath[0]);
        currentStep++;

        situation->updateQueens();
        situation->printField();

        if (currentStep % 2 == 1) {
            foundPath = AiSolver::solveMinimaxAlphaBetaOptimized(*situation, target, minimaxDepth, evaluateScoreMinimax);
        } else {
            while(1) {
            //  foundPath = AiSolver::solveDepthSearchWithScoreFunc(*situation, target, 200, evaluateScore);
                foundPath = AiSolver::solveManConsole(*situation);
                if (foundPath.size() != 0) {
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
    delete situation;
    return 0;
}
