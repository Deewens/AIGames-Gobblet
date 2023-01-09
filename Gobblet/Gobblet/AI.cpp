#include "AI.hpp"

#include "Board.hpp"

void AI::makeBestMove(Board& t_board, Entity t_AIPlayer)
{
    int bestScore = INT_MIN;
    Move bestMove;
    
    const auto& gobbletStacks = t_board.getGobbletStacks();
    
    for (const auto& stack : gobbletStacks)
    {
        for (const auto& movement : getAllLegalMoves(t_board, t_AIPlayer))
        {
            const int score = minimax(t_board, 4, INT_MIN, INT_MAX, t_AIPlayer);
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = movement;
            }
        }
    }

    t_board.moveGobblet(bestMove.gobbletStack, bestMove.to);
}

int AI::minimax(Board t_board, int t_depth, int alpha, int beta, Entity t_player)
{
    if (t_depth == 0) // Should also return the score if game is over (if player is gonna win)
    {
        return evaluateScore(t_board, t_player);
    }

    if (t_player.getType() == PlayerType::Max) // Max player
    {
        auto legalMoves = getAllLegalMoves(t_board, t_player);
        for (auto& move : legalMoves)
        {
            t_board.moveGobblet(move.gobbletStack, move.to);
            beta = std::min(beta, minimax(t_board, t_depth - 1, alpha, beta, t_player));
            if (alpha >= beta)
            {
                return beta; // Prune
            }
        }
        return beta;
    }
    else // Min
    {
        auto legalMoves = getAllLegalMoves(t_board, t_player);
        for (auto& move : legalMoves)
        {
            t_board.moveGobblet(move.gobbletStack, move.to);
            alpha = std::max(alpha, minimax(t_board, t_depth - 1, alpha, beta, t_player));
            if (alpha >= beta)
            {
                return alpha; // Prune
            }
        }

        return alpha;
    }
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
                                    Move(t_currentPlayer, *tile.gobbletStack.lock(), sf::Vector2i(y, x), {i, j}));
                            }
                            else
                            {
                                const auto currentStack = tile2.gobbletStack.lock();
                                if (currentStack->top().getSize() > tile.gobbletStack.lock()->top().getSize())
                                {
                                    possibleMoves.push_back(
                                        Move(t_currentPlayer, *tile.gobbletStack.lock(), sf::Vector2i(y, x), {i, j}));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    const auto allGobbletStacks = t_board.getGobbletStacks();
    for (const auto& stack : allGobbletStacks)
    {
        if (stack.isExternalStack())
        {
            for (int y = 0; y < grid.size(); y++)
            {
                for (int x = 0; x < grid[y].size(); x++)
                {
                    auto tile = grid[y][x];
                    if (tile.gobbletStack.expired())
                    {
                        possibleMoves.push_back(
                            Move(t_currentPlayer, stack, std::nullopt, {y, x}));
                    }
                    else
                    {
                        auto currentStack = tile.gobbletStack.lock();
                        if (currentStack->top().getSize() > stack.top().getSize())
                        {
                            possibleMoves.push_back(
                                Move(t_currentPlayer, stack, std::nullopt, {y, x}));
                        }
                    }
                }
            }
        }
    }

    return possibleMoves;
}
