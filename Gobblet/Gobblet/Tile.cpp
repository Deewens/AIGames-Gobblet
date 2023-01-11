#include "Tile.h"

#include "Gobblet.hpp"

Tile::Tile()
{
    m_tileShape.setFillColor(sf::Color(139, 69, 19));
    m_tileShape.setOutlineColor(sf::Color::Black);
    m_tileShape.setOutlineThickness(2);
    m_tileShape.setSize(sf::Vector2f(m_tileSize, m_tileSize));
}

Tile::Tile(int row, int col, int t_tileSize)
{
    // RGB Color brown
    m_tileShape.setFillColor(sf::Color(139, 69, 19));
    m_tileShape.setOutlineColor(sf::Color::Black);
    m_tileShape.setOutlineThickness(2);
    m_tileShape.setSize(sf::Vector2f(t_tileSize, t_tileSize));

    m_gridCoordinate = sf::Vector2i(row, col);
    std::cout << row << ", " << col << std::endl;
}

Tile::~Tile()
{
}

void Tile::Update(sf::RectangleShape t_mouse)
{
}

sf::Vector2i Tile::GetGridCoordinate()
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
}

sf::Vector2f Tile::getCenter() const
{
    return m_tileShape.getPosition() + m_tileShape.getSize() / 2.f;
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_tileShape, states);
}

bool operator==(const Tile& t_lhs, const Tile& t_rhs)
{
    return t_lhs.m_gridCoordinate == t_rhs.m_gridCoordinate
        && t_lhs.m_tileSize == t_rhs.m_tileSize;
}

bool operator!=(const Tile& t_lhs, const Tile& t_rhs)
{
    return !(t_lhs == t_rhs);
}
