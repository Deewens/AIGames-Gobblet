#pragma once
#include <vector>

#include "Move.hpp"

class Board;
class Entity;

class AI
{
public:
    static Move findBestMove(Board t_board, Entity t_AIPlayer);
    
    static int minimax(Board t_board, int t_depth, int alpha, int beta, const Entity& t_player);

    static int evaluateScore(const Board& t_board, const Entity& t_player);

    static std::vector<Move> getAllLegalMoves(const Board& t_board, const Entity& t_currentPlayer);
};

