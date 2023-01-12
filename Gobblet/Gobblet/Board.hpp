#pragma once

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Grid.h"

class GobbletStack;

/**
 * \brief Board game state and logic
 */
class Board final : public sf::Drawable
{
public:
    explicit Board(sf::RenderWindow& t_window);

    void update(sf::Time t_deltaTime);
    /**
     * \brief Choose the gobblet to be moved. The gobblet is selected from mouse position
     * \return flag used for controlling code flow. If true, nothing should be called after this method, and the calling method should immediately break or return. (e.g.: should not called placeGobblet after this method if it return true)
     */
    bool chooseGobblet();

    /**
     * \brief Switch the current playing player (from min to max or max to min)
     */
    void switchCurrentPlayer();
    /**
     * \brief Place the chosen gobblet somewhere in the grid according to mouse position
     * \return flag used for controlling code flow. If true, nothing should be called after this method, and the calling method should immediately break or return
     */
    bool placeGobblet();

    void processMouse(sf::Event t_event);

    /// <summary>
    /// This checks if there are 4 gobblets in a row/col or diagonal
    /// </summary>
    bool checkWinCondition(sf::Color t_color);

    /// <summary>
    ///Checks if either players can gobble using their reserves if there are 3 gobblets of the same colour in a row
    /// </summary>
    /// <param name="a">The row/diagonalA array of colors</param>
    /// <param name="b">The col/diagonalB array of colors</param>
    /// <param name="t_color"></param>
    /// <returns>Returns if either player is able to use the ability</returns>
    bool checkReserveUsage(sf::Color a[], sf::Color b[], sf::Color t_color);

    void checkTieCondition();

    Grid& getGrid();
    const Grid& getGrid() const;
    Entity& getOpponent(const Entity& t_entity);

    /**
     * \brief Move a gobblet from one stack to another
     * \param t_fromStack The stack to move the gobblet from
     * \param t_toStack the stack to move the gobblet to
     */
    static void moveGobblet(GobbletStack& t_fromStack, GobbletStack& t_toStack);

    /**
     * \brief Given a stack, return the pointer to one of the stack stored in the board or the external stack list
     * \param t_stack a stack to search
     * \return pointer to the stack
     */
    GobbletStack* findGobbletStack(const GobbletStack& t_stack);

    /**
     * \brief Get whose turn it is
     * \return raw pointer to the current playing player
     */
    Entity* getCurrentPlayer();
    
    Entity& getMinPlayer();
    Entity& getMaxPlayer();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    /**
     * \brief Control the possible action that the player can do with the mouse
     */
    enum class ActionState
    {
        ChooseGobblet,
        PlaceGobblet,
        PlaceBoardGobblet
    };

    sf::RenderWindow& m_window;
    sf::FloatRect m_mousePosition;

    Grid m_grid;
    GobbletStack* m_activeStack = nullptr;

    ActionState m_gobbletActionState = ActionState::ChooseGobblet;

    Entity* m_currentPlayer = nullptr;

    Entity m_maxPlayer;
    Entity m_minPlayer;

    int m_sameActionCount;
};

/*
Rules to implement:
Gobblet clicked on board must be moved. Done
If the same or similar move is made 3 times between players, it's a draw.
If there are 3 Gobblets of the same colour in a row, opposing player may gobble up a gobblet from their reserves
4 in a row of the same colour is a win. Done

*/