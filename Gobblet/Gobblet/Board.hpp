﻿#pragma once

#include "Grid.h"
#include "Entity.h"

class Board final : public sf::Drawable
{
public:
    explicit Board(sf::RenderWindow& t_window);

    void update(sf::Time t_deltaTime);
    void deactivateClickedGobblet();

    /**
     * \brief Choose the gobblet to be moved. The gobblet is selected from mouse position
     * \return flag used for controlling code flow. If true, nothing should be called after this method, and the calling method should immediately break or return. (e.g.: should not called placeGobblet after this method if it return true)
     */
    bool chooseGobblet();
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
    bool CheckWinCondition(sf::Color t_color);

    /// <summary>
    ///Checks if either players can gobble using their reserves if there are 3 gobblets of the same colour in a row
    /// </summary>
    /// <param name="a">The row/diagonalA array of colors</param>
    /// <param name="b">The col/diagonalB array of colors</param>
    /// <param name="t_color"></param>
    /// <returns>Returns if either player is able to use the ability</returns>
    bool CheckReserveUsage(sf::Color a[], sf::Color b[], sf::Color t_color);

    void CheckTieCondition();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    enum class ActionState
    {
        ChooseGobblet,
        PlaceGobblet,
        PlaceBoardGobblet
    };

    sf::RenderWindow& m_window;
    sf::FloatRect m_mousePosition;

    Grid m_grid;

    std::vector<Gobblet*> m_gobblets;

    ActionState m_gobbletActionState = ActionState::ChooseGobblet;

    Entity* m_currentPlayer;

    Entity m_maxPlayer;
    Entity m_minPlayer;

    Gobblet* m_clickedGobblet = nullptr;

    int m_sameActionCount;
};

/*
Rules to implement:
Gobblet clicked on board must be moved. Done
If the same or similar move is made 3 times between players, it's a draw.
If there are 3 Gobblets of the same colour in a row, opposing player may gobble up a gobblet from their reserves
4 in a row of the same colour is a win. Done

*/