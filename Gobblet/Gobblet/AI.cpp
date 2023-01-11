#include "AI.hpp"

#include <algorithm>

#include "Board.hpp"


Move AI::findBestMove(Board& t_board, Entity t_AIPlayer)
{
    int bestScore = INT_MIN;
    Move* bestMove = nullptr;

    Board copyBoard = t_board;

    const auto& gobbletStacks = t_board.getGobbletStacks();

    for (const auto& stack : gobbletStacks)
    {
        for (const auto& movement : getAllLegalMoves(t_board, t_AIPlayer))
        {
            const int score = minimax(copyBoard, 3, INT_MIN, INT_MAX, t_AIPlayer);
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = new Move(movement);
            }
        }
    }

    Move finalMove = *bestMove;
    delete bestMove;

    return finalMove;
}

int AI::minimax(Board t_board, int t_depth, int alpha, int beta, Entity t_player)
{
    int bestValue = 0;
    // print alpha and beta
    std::cout << "Alpha: " << alpha << " Beta: " << beta << std::endl;

    if (t_depth == 0 || t_board.CheckWinCondition(t_player.getColor()))
    // Should also return the score if game is over (if player is gonna win)
    {
        bestValue = evaluateScore(t_board, t_player);
    }
    else if (t_player.getType() == PlayerType::Max) // Max player
    {
        bestValue = alpha;
        
        auto legalMoves = getAllLegalMoves(t_board, t_player);
        for (auto& move : legalMoves)
        {
            t_board.moveGobblet(move.gobblet, &move.toTile);
            bestValue = std::max(bestValue, minimax(t_board, t_depth - 1, bestValue, beta,
                                                    t_board.getOpponent(t_player)));

            t_board.moveGobblet(move.gobblet, &move.fromTile.value());
            
            //t_board.moveGobblet(move.toStack, move.toTile);

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
            t_board.moveGobblet(move.gobblet, &move.toTile);
            bestValue = std::min(bestValue, minimax(t_board, t_depth - 1, alpha, bestValue,
                                                    t_board.getOpponent(t_player)));
            t_board.moveGobblet(move.gobblet, &move.fromTile.value());

            // Move it back to its initial position
            //t_board.moveGobblet(move.gobblet, move.fromStack.getGridPosition());
            if (bestValue <= alpha)
            {
                break;
            }
        }
    }

    return bestValue;
}

int AI::evaluateScore(Board& t_board, Entity& t_player)
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
            if (!tile.gobbletStack.expired())
            {
                if (tile.gobbletStack.lock()->top().getColor() == t_player.getColor())
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
            if (!tile.gobbletStack.expired())
            {
                if (tile.gobbletStack.lock()->top().getColor() == t_player.getColor())
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

        if (t_player.getType() == PlayerType::Max)
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


std::vector<Move> AI::getAllLegalMoves(const Board& t_board, Entity t_currentPlayer)
{
    // Get current board state
    const auto grid = t_board.getGrid().getGridArray();

    // Get all possible moves
    std::vector<Move> possibleMoves;
    for (int y = 0; y < grid.size(); y++)
    {
        for (int x = 0; x < grid[y].size(); x++)
        {
            auto tile = grid[y][x];
            if (!tile.gobbletStack.expired())
            {
                auto movingGobblet = tile.gobbletStack.lock()->top();
                if (tile.gobbletStack.lock()->top().getColor() == t_currentPlayer.getColor())
                {
                    for (int i = 0; i < grid.size(); i++)
                    {
                        for (int j = 0; j < grid[i].size(); j++)
                        {
                            auto tile2 = grid[i][j];
                            if (tile2.gobbletStack.expired())
                            {
                                Move move;
                                move.gobblet = movingGobblet;
                                move.fromStack = *tile.gobbletStack.lock();
                                move.fromTile = tile;
                                //move.toStack = *tile2.gobbletStack.lock();
                                move.toTile = tile2;

                                possibleMoves.push_back(move);
                            }
                            else
                            {
                                auto currentStack = tile2.gobbletStack.lock();
                                if (currentStack->top().getSize() > tile.gobbletStack.lock()->top().getSize())
                                {
                                    Move move;
                                    move.gobblet = movingGobblet;
                                    move.fromStack = *tile.gobbletStack.lock();
                                    move.fromTile = tile;
                                    move.toStack = *tile2.gobbletStack.lock();
                                    move.toTile = tile2;

                                    possibleMoves.push_back(move);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    const auto& allGobbletStacks = t_board.getGobbletStacks();
    for (const auto stack : allGobbletStacks)
    {
        if (stack->isExternalStack)
        {
            auto movingGobblet = stack->top();
            for (int y = 0; y < grid.size(); y++)
            {
                for (int x = 0; x < grid[y].size(); x++)
                {
                    auto tile = grid[y][x];
                    if (tile.gobbletStack.expired())
                    {
                        Move move;
                        move.gobblet = movingGobblet;
                        move.fromStack = *stack;
                        move.fromTile = std::nullopt;
                        //move.toStack = *tile.gobbletStack.lock();
                        move.toTile = tile;

                        possibleMoves.push_back(move);
                    }
                    else
                    {
                        auto currentStack = tile.gobbletStack.lock();
                        if (currentStack->top().getSize() > stack->top().getSize())
                        {
                            Move move;
                            move.gobblet = movingGobblet;
                            move.fromStack = *stack;
                            move.fromTile = std::nullopt;
                            move.toStack = *tile.gobbletStack.lock();
                            move.toTile = tile;

                            possibleMoves.push_back(move);
                        }
                    }
                }
            }
        }
    }

    return possibleMoves;
}
