#pragma once
#include <vector>

#include "Move.hpp"
#include "MovePtr.hpp"

class Board;
class Entity;

class AI
{
public:
    /**
     * \brief Find the best move for the AI to make.
     * \param t_board Board corresponding to the current game state (passed as a copy to not update the real board).
     * \return The best move for the AI to make.
     */
    static Move findBestMove(Board t_board);

    /**
     * \brief Minimax algorithm
     * \param t_board Game state to evaluate.
     * \param t_depth
     * \param alpha 
     * \param beta 
     * \param t_isMaximising if the current player is max/min.
     * \param bestMove Reference to the best move found so far.
     * \return best score found by minimax
     */
    static int minimax(Board& t_board, int t_depth, int alpha, int beta, bool t_isMaximising, MovePtr& bestMove);

    /**
     * \brief Calculate a score for the board for a specific player.
     * \param t_board Game state
     * \param t_player player to evaluate the score for
     * \return calculated score of the player
     */
    static int evaluateScore(const Board& t_board, const Entity& t_player);

    /**
     * \brief Search for all the possible moves, that are legal in the game of Gobblet.
     * \param t_board 
     * \param t_currentPlayer 
     * \return list of legal moves
     */
    static std::vector<MovePtr> getAllLegalMoves(Board& t_board, Entity& t_currentPlayer);
};

