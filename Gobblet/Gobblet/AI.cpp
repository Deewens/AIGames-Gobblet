#include "AI.h"

#include "Board.hpp"
#include "Entity.h"
#include "Grid.h"
#include "Move.hpp"

Move AI::findBestMove(Board t_board)
{
    MovePtr bestMove{};

    const int score = minimax(t_board, 4, INT_MIN, INT_MAX, false, bestMove);

    std::cout << score << std::endl;
    //return bestMove;
    return Move(*bestMove.gobblet, *bestMove.fromStack, *bestMove.toStack);
}

int AI::minimax(Board& t_board, int t_depth, int alpha, int beta, bool t_isMaximising, MovePtr& bestMove)
{
    int bestValue = 0;
    // print alpha and beta
    //std::cout << "Alpha: " << alpha << " Beta: " << beta << std::endl;

    Entity* currentPlayer;
    if (t_isMaximising)
    {
        currentPlayer = &t_board.getMaxPlayer();
    }
    else
    {
        currentPlayer = &t_board.getMinPlayer();
    }

    if (t_depth == 0 || t_board.checkWinCondition(currentPlayer->getColor()))
    // Should also return the score if game is over (if player is gonna win)
    {
        bestValue = evaluateScore(t_board, *currentPlayer);
    }
    else if (t_isMaximising) // Max player
    {
        bestValue = alpha;

        const std::vector<MovePtr>& legalMoves = getAllLegalMoves(t_board, *currentPlayer);
        for (auto& move : legalMoves)
        {
            Board::moveGobblet(*move.fromStack, *move.toStack);
            bestValue = std::max(bestValue, minimax(t_board, t_depth - 1, bestValue, beta, false, bestMove));
            Board::moveGobblet(*move.toStack, *move.fromStack);

            if (beta <= bestValue)
            {
                bestMove = move;
                break;
            }
        }
    }
    else // Min
    {
        bestValue = beta;

        const std::vector<MovePtr>& legalMoves = getAllLegalMoves(t_board, *currentPlayer);
        for (auto& move : legalMoves)
        {
            Board::moveGobblet(*move.fromStack, *move.toStack);
            bestValue = std::min(bestValue, minimax(t_board, t_depth - 1, alpha, bestValue,
                                                    true, bestMove));
            Board::moveGobblet(*move.toStack, *move.fromStack);

            if (bestValue <= alpha)
            {
                bestMove = move;
                break;
            }
        }
    }

    return bestValue;
}

int AI::evaluateScore(const Board& t_board, const Entity& t_player)
{
    int totalScore = 0;

    const auto& grid = t_board.getGrid().getGridArray();
    for (int y = 0; y < grid.size(); y++)
    {
        int numberInRow = 0;
        int numberInCol = 0;

        int rowScoreToAdd = 0;
        int colScoreToAdd = 0;

        for (int x = 0; x < grid[y].size(); x++)
        {
            auto& tile = grid[y][x];
            if (!tile.getStack().isEmpty())
            {
                if (tile.getStack().top().getColor() == t_player.getColor())
                {
                    numberInRow++;

                    if (numberInRow == 1)
                    {
                        colScoreToAdd = 1;
                    }
                    else if (numberInRow == 2)
                    {
                        colScoreToAdd = 3;
                    }
                    else if (numberInRow == 3)
                    {
                        colScoreToAdd = 5;
                    }
                    else if (numberInRow == 4)
                    {
                        colScoreToAdd = 1000;
                    }
                }
            }

            auto& tile2 = grid[x][y]; // Store the tile in the column
            if (!tile2.getStack().isEmpty())
            {
                if (tile2.getStack().top().getColor() == t_player.getColor())
                {
                    numberInCol++;

                    if (numberInCol == 1)
                    {
                        rowScoreToAdd = 1;
                    }
                    else if (numberInCol == 2)
                    {
                        rowScoreToAdd = 3;
                    }
                    else if (numberInCol == 3)
                    {
                        rowScoreToAdd = 5;
                    }
                    else if (numberInCol == 4)
                    {
                        rowScoreToAdd = 1000;
                    }
                }
            }
        }


        totalScore += colScoreToAdd + rowScoreToAdd;
    }

    return totalScore;
}

std::vector<MovePtr> AI::getAllLegalMoves(Board& t_board, Entity& t_currentPlayer)
{
    // Get current board state
    auto& grid = t_board.getGrid().getGridArray();

    // Get all possible moves
    std::vector<MovePtr> possibleMoves;
    for (int y = 0; y < grid.size(); y++)
    {
        for (int x = 0; x < grid[y].size(); x++)
        {
            auto& tile = grid[y][x];
            if (!tile.getStack().isEmpty())
            {
                auto& movingGobblet = tile.getStack().top();
                if (tile.getStack().top().getColor() == t_currentPlayer.getColor())
                {
                    for (int i = 0; i < grid.size(); i++)
                    {
                        for (int j = 0; j < grid[i].size(); j++)
                        {
                            auto& tile2 = grid[i][j];
                            if (tile2.getStack().isEmpty())
                            {
                                MovePtr move{};
                                move.gobblet = &movingGobblet;
                                move.fromStack = &tile.getStack();
                                move.toStack = &tile2.getStack();

                                possibleMoves.push_back(move);
                            }
                            else
                            {
                                auto& currentStack = tile2.getStack();
                                if (currentStack.top().getSize() > tile.getStack().top().getSize())
                                {
                                    MovePtr move{};
                                    move.gobblet = &movingGobblet;
                                    move.fromStack = &tile.getStack();
                                    move.toStack = &tile2.getStack();

                                    possibleMoves.push_back(move);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    std::vector<GobbletStack>& externalStacks = t_currentPlayer.getExternalStacks();
    for (auto& stack : externalStacks)
    {
        auto& movingGobblet = stack.top();
        for (int y = 0; y < grid.size(); y++)
        {
            for (int x = 0; x < grid[y].size(); x++)
            {
                auto& tile = grid[y][x];
                if (tile.getStack().isEmpty())
                {
                    MovePtr move{};
                    move.gobblet = &movingGobblet;
                    move.fromStack = &stack;
                    move.toStack = &tile.getStack();

                    possibleMoves.push_back(move);
                }
                else
                {
                    auto& currentStack = tile.getStack();
                    if (currentStack.top().getSize() > stack.top().getSize())
                    {
                        MovePtr move{};
                        move.gobblet = &movingGobblet;
                        move.fromStack = &stack;
                        move.toStack = &tile.getStack();

                        possibleMoves.push_back(move);
                    }
                }
            }
        }
    }

    return possibleMoves;
}
