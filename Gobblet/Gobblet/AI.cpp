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
    int bestValue = evaluateScore(t_board, t_player);
    
    if (t_depth == 0 || bestValue >= 1000) // Should also return the score if game is over (if player is gonna win)
    {
        return bestValue;
        //bestValue = evaluateScore(t_board, t_player);
    }

    // print alpha and beta
    std::cout << "Alpha: " << alpha << " Beta: " << beta << std::endl;

    if (t_player.getType() == PlayerAIType::Max) // Max player
    {
        auto legalMoves = getAllLegalMoves(t_board, t_player);
        //Board boardCopy = t_board;
        for (auto& move : legalMoves)
        {
            t_board.moveGobblet(move.fromStack, move.toPosition);
            alpha = std::max(alpha, minimax(t_board, t_depth - 1, alpha, beta, t_board.getOpponent(t_player)));
            t_board.moveGobblet(move.gobblet, std::nullopt);

            if (alpha >= beta)
            {
                return alpha; // Prune
            }
        }
        return alpha;
    }
    else // Min
    {
        //bestValue = beta;
        
        auto legalMoves = getAllLegalMoves(t_board, t_player);
        //Board boardCopy = t_board;
        for (auto& move : legalMoves)
        {
            t_board.moveGobblet(move.fromStack, move.toPosition);
            beta = std::min(beta, minimax(t_board, t_depth - 1, alpha, bestValue, t_board.getOpponent(t_player)));

            // Move it back to its initial position
            t_board.moveGobblet(move.gobblet, move.fromStack.getGridPosition());
            if (alpha >= beta)
            {
                return beta; // Prune
            }
        }

        return beta;
    }

    //return bestValue;
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

        totalScore += colScoreToAdd + rowScoreToAdd;
    }

    return totalScore;
}

std::vector<Move> AI::getAllLegalMoves(Board& t_board, Entity t_currentPlayer)
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
                                possibleMoves.push_back(
                                    Move(t_currentPlayer, movingGobblet, *tile.gobbletStack.lock(), {i, j}));
                            }
                            else
                            {
                                auto currentStack = tile2.gobbletStack.lock();
                                if (currentStack->top().getSize() > tile.gobbletStack.lock()->top().getSize())
                                {
                                    possibleMoves.push_back(
                                        Move(t_currentPlayer, movingGobblet, *tile.gobbletStack.lock(), {i, j}));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    const auto allGobbletStacks = t_board.getGobbletStacks();
    for (const auto stack : allGobbletStacks)
    {
        if (stack->isExternalStack())
        {
            auto movingGobblet = stack->top();
            for (int y = 0; y < grid.size(); y++)
            {
                for (int x = 0; x < grid[y].size(); x++)
                {
                    auto tile = grid[y][x];
                    if (tile.gobbletStack.expired())
                    {
                        possibleMoves.push_back(
                            Move(t_currentPlayer, movingGobblet, *stack, {y, x}));
                    }
                    else
                    {
                        auto currentStack = tile.gobbletStack.lock();
                        if (currentStack->top().getSize() > stack->top().getSize())
                        {
                            possibleMoves.push_back(
                                Move(t_currentPlayer, movingGobblet, *stack, {y, x}));
                        }
                    }
                }
            }
        }
    }

    return possibleMoves;
}
