#pragma once
#include "Entity.hpp"
#include "Move.hpp"
#include "PlayerGameState.hpp"

class Board;
class Entity;

class AI
{
    int minimax(Board t_board, int t_depth, int alpha, int beta, Entity t_player);

    int evaluateScore(Board& t_board, Entity& t_player);

private:
    std::vector<Move> getAllLegalMoves(Board& t_board, Entity t_currentPlayer);
};
