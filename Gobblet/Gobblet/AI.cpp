#include "AI.hpp"

#include "Board.hpp"

int AI::evaluateScore(Board& t_board, Entity& t_player)
{
    int totalScore = 0;

    auto grid = t_board.getGrid().getGridArray();
    for (int y = 0; y < grid.size(); y++)
    {
        int numberInCol = 0;
        int numberInRow = 0;

        int scoreToAdd = 0;

        for (int x = 0; x < grid[y].size(); x++)
        {
            auto tile = grid[y][x];
            if (!tile.gobbletStack.expired())
            {
                if (tile.gobbletStack.lock()->top().getColor() == t_player.GetColor())
                {
                    numberInCol++;
                    
                    if (numberInCol == 1)
                    {
                        scoreToAdd = 1;
                    }
                    else if (numberInCol == 2)
                    {
                        scoreToAdd = 3;
                    }
                    else if (numberInCol == 3)
                    {
                        scoreToAdd = 5;
                    }
                    else if (numberInCol == 4)
                    {
                        scoreToAdd = 1000;
                    }
                }
            }
        }

        totalScore += scoreToAdd;
    }
}
