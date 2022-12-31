#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

#include "Board.hpp"
#include "Gobblet.hpp"
#include "Grid.h"

class Game
{
public:
    Game();
    ~Game();
    /// <summary>
    /// main method for game
    /// </summary>
    void run();

private:
    void processEvents();
    void processKeys(sf::Event t_event);
    void update(sf::Time t_deltaTime);
    void render();
    
    sf::RenderWindow m_window; // main SFML window

    Board m_board;
    
    sf::Font m_ArialBlackfont; // font used by message
    bool m_exitGame; // control exiting game
};

#endif // !GAME_HPP
