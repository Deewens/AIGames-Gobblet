#include "Tile.h"

#include <iostream>

#include "Gobblet.hpp"

Tile::Tile(): m_stack(false)
{
    m_tileShape.setFillColor(sf::Color(139, 69, 19));
    m_tileShape.setOutlineColor(sf::Color::Black);
    m_tileShape.setOutlineThickness(2);
    m_tileShape.setSize(sf::Vector2f(m_tileSize, m_tileSize));
}


Tile::Tile(int row, int col, int t_tileSize): m_stack(false)
{
    // RGB Color brown
    m_tileShape.setFillColor(sf::Color(139, 69, 19));
    m_tileShape.setOutlineColor(sf::Color::Black);
    m_tileShape.setOutlineThickness(2);
    m_tileShape.setSize(sf::Vector2f(t_tileSize, t_tileSize));

    m_gridCoordinate = sf::Vector2f(row, col);
    std::cout << row << ", " << col << std::endl;
}

Tile::~Tile()
{
}

void Tile::Update(sf::RectangleShape t_mouse)
{
}

sf::Vector2f Tile::GetGridCoordinate()
{
    return m_gridCoordinate;
}

sf::RectangleShape& Tile::GetShape()
{
    return m_tileShape;
}

void Tile::SetPosition(int t_x, int t_y)
{
    m_tileShape.setPosition(sf::Vector2f(t_x, t_y));
    m_stack.setPosition(getCenter());
}

sf::Vector2f Tile::getCenter() const
{
    return m_tileShape.getPosition() + m_tileShape.getSize() / 2.f;
}

GobbletStack& Tile::getStack()
{
    return m_stack;
}

const GobbletStack& Tile::getStack() const
{
    return m_stack;
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_tileShape, states);
}
