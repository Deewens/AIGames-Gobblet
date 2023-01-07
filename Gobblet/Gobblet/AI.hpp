#pragma once
#include "PlayerGameState.hpp"

class Board;
class Entity;

class AI
{
    void minimax(Board t_board, int t_depth, Entity t_maximisingPlayer);

    int evaluateScore(Board& t_board, Entity& t_player);
};
