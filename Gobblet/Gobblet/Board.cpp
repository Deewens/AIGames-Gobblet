#include "Board.hpp"

#include "Gobblet.hpp"

Board::Board(sf::RenderWindow& t_window) : m_window(t_window)
{
    const sf::Vector2f gridPosition = m_grid.getPosition();
    const sf::Vector2f gridSize = m_grid.getSize();
    
    float YPosition = gridPosition.y;
    for (int i = 0; i < 3; i++)
    {
        GobbletStack blackStack;
        GobbletStack whiteStack;

        YPosition = YPosition + 50;
        
        for (int j = 1; j <= 4; j++)
        {
            blackStack.add(Gobblet(Gobblet::Color::Black, j, m_grid));
            blackStack.setPosition({gridPosition.x - 50, YPosition}); // 50px - left side of the grid

            whiteStack.add(Gobblet(Gobblet::Color::White, j, m_grid));
            whiteStack.setPosition({gridPosition.x + (gridSize.x + 50), YPosition}); // 50px - right side of the grid
        }
        
        m_gobbletStacks.push_back(blackStack);
        m_gobbletStacks.push_back(whiteStack);
    }
}

void Board::update(sf::Time t_deltaTime)
{
    m_grid.update(m_window);
}

void Board::processMouse(sf::Event t_event)
{
    m_grid.mouseEvents(t_event);
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_grid, states);

    for (const auto& stack : m_gobbletStacks)
    {
        target.draw(stack, states);
    }
}
