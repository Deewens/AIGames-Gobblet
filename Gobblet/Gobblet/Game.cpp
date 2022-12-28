/// <summary>
/// Authors: Adrien Dudon and Leo Chen
/// Date started: 25/11/2022
/// </summary>

#include "Game.h"
#include <iostream>

/// <summary>
/// default constructor
/// setup the window properties
/// </summary>
Game::Game() :
    m_gobblet(std::make_shared<Gobblet>(Gobblet::Color::White, 3, m_grid)),
    m_gobblet2(std::make_shared<Gobblet>(Gobblet::Color::Black, 4, m_grid)),
    m_window{
        sf::VideoMode{sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2, 32U},
        "Gobblet Game"
    }, //when true game will exit
    m_exitGame{false}
{
    // Just testing the Gobblet class
    // TODO: to be removed
    // ================================
    m_gobblet->setGridCoordinates(sf::Vector2i(0, 1));
    //m_gobblet.setGridCoordinates(std::nullopt);
    
    //m_gobblet2->setGridCoordinates(sf::Vector2i(0, 1));
    // ================================

}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game() = default;


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const float fps{60.0f};
    sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
    while (m_window.isOpen())
    {
        processEvents(); // as many as possible
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents(); // at least 60 fps
            update(timePerFrame); //60 fps
        }
        render(); // as many as possible
    }
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
    sf::Event newEvent;
    while (m_window.pollEvent(newEvent))
    {
        if (sf::Event::Closed == newEvent.type) // window message
        {
            m_exitGame = true;
        }
        if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
        {
            processKeys(newEvent);
        }

        m_grid.MouseEvents(newEvent);
    }
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
    if (sf::Keyboard::Escape == t_event.key.code)
    {
        m_exitGame = true;
    }
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
    if (m_exitGame)
    {
        m_window.close();
    }

    m_grid.Update();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
    m_window.clear(sf::Color::Green);
    m_grid.Draw(m_window);
    
    m_window.draw(*m_gobblet);
    m_window.draw(*m_gobblet2);

    m_window.display();
}
