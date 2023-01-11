#include "AI.h"

#include "Board.hpp"
#include "Entity.h"
#include "Grid.h"

Move AI::findBestMove(Board t_board, Entity t_AIPlayer)
{
    int bestScore = INT_MIN;
    const Move* bestMove = nullptr;


    for (const auto& movement : getAllLegalMoves(t_board, t_AIPlayer))
    {
        const int score = minimax(t_board, 3, INT_MIN, INT_MAX, t_AIPlayer);
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = new Move(movement);
        }
    }

    const Move finalMove = *bestMove;
    delete bestMove;

    return finalMove;
}

int AI::minimax(Board t_board, int t_depth, int alpha, int beta, const Entity& t_player)
{
    int bestValue = 0;
    // print alpha and beta
    std::cout << "Alpha: " << alpha << " Beta: " << beta << std::endl;

    if (t_depth == 0 || t_board.checkWinCondition(t_player.getColor()))
    // Should also return the score if game is over (if player is gonna win)
    {
        bestValue = evaluateScore(t_board, t_player);
    }
    else if (t_player.getType() == Entity::Type::Max) // Max player
    {
        bestValue = alpha;

        auto legalMoves = getAllLegalMoves(t_board, t_player);
        for (auto& move : legalMoves)
        {
            Board::moveGobblet(*move.fromStack, *move.toStack);
            bestValue = std::max(bestValue, minimax(t_board, t_depth - 1, bestValue, beta,
                                                    t_board.getOpponent(t_player)));

            Board::moveGobblet(*move.toStack, *move.fromStack);

            if (beta <= bestValue)
            {
                break;
            }
        }
    }
    else // Min
    {
        bestValue = beta;

        auto legalMoves = getAllLegalMoves(t_board, t_player);
        for (auto& move : legalMoves)
        {
            Board::moveGobblet(*move.fromStack, *move.toStack);
            bestValue = std::min(bestValue, minimax(t_board, t_depth - 1, alpha, bestValue,
                                                    t_board.getOpponent(t_player)));
            Board::moveGobblet(*move.toStack, *move.fromStack);

            if (bestValue <= alpha)
            {
                break;
            }
        }
    }

    return bestValue;
}

int AI::evaluateScore(const Board& t_board, const Entity& t_player)
{
    int totalScore = 0;

    const auto grid = t_board.getGrid().getGridArray();
    for (int y = 0; y < grid.size(); y++)
    {
        int numberInRow = 0;
        int numberInCol = 0;

        int rowScoreToAdd = 0;
        int colScoreToAdd = 0;

        for (int x = 0; x < grid[y].size(); x++)
        {
            auto tile = grid[y][x];
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

            tile = grid[x][y]; // Store the tile in the column
            if (!tile.getStack().isEmpty())
            {
                if (tile.getStack().top().getColor() == t_player.getColor())
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

        if (t_player.getType() == Entity::Type::Max)
        {
            totalScore += colScoreToAdd + rowScoreToAdd;
        }
        else
        {
            totalScore -= colScoreToAdd - rowScoreToAdd;
        }
    }

    return totalScore;
}

std::vector<Move> AI::getAllLegalMoves(const Board& t_board, const Entity& t_currentPlayer)
{
    // Get current board state
    auto& grid = t_board.getGrid().getGridArray();

    // Get all possible moves
    std::vector<Move> possibleMoves;
    for (int y = 0; y < grid.size(); y++)
    {
        for (int x = 0; x < grid[y].size(); x++)
        {
            auto tile = grid[y][x];
            if (!tile.getStack().isEmpty())
            {
                auto movingGobblet = tile.getStack().top();
                if (tile.getStack().top().getColor() == t_currentPlayer.getColor())
                {
                    for (int i = 0; i < grid.size(); i++)
                    {
                        for (int j = 0; j < grid[i].size(); j++)
                        {
                            auto tile2 = grid[i][j];
                            if (tile2.getStack().isEmpty())
                            {
                                Move move;
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
                                    Move move;
                                    move.gobblet = &movingGobblet;
                                    move.fromStack = &tile.getStack();
                                    move.toStack = &currentStack;

                                    possibleMoves.push_back(move);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    const auto& externalStacks = t_currentPlayer.getExternalStacks();
    for (auto stack : externalStacks)
    {
        auto movingGobblet = stack.top();
        for (int y = 0; y < grid.size(); y++)
        {
            for (int x = 0; x < grid[y].size(); x++)
            {
                auto tile = grid[y][x];
                if (tile.getStack().isEmpty())
                {
                    Move move{};
                    move.gobblet = &movingGobblet;
                    move.fromStack = &stack;
                    move.toStack = &tile.getStack();

                    possibleMoves.push_back(move);
                }
                else
                {
                    auto currentStack = tile.getStack();
                    if (currentStack.top().getSize() > stack.top().getSize())
                    {
                        Move move{};
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
