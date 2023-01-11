#include "Gobblet.hpp"

#include "Grid.h"

Gobblet::Gobblet(const sf::Color& t_color, int t_size) :
    m_size(t_size),
    m_shape(static_cast<float>(t_size) * m_sizeFactor)
{
    verifySize();

    if (t_color == sf::Color::White)
    {
        m_color = sf::Color::White;
    }
    else
    {
        m_color = sf::Color::Black;
    }

    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
    m_shape.setFillColor(m_color);
}

int Gobblet::getSize() const
{
    return m_size;
}

const sf::Color& Gobblet::getColor() const
{
    return m_color;
}

std::optional<sf::Vector2i> Gobblet::getGridCoordinates() const
{
    return m_gridCoordinates;
}

void Gobblet::setGridCoordinates(const Grid& t_grid, const std::optional<sf::Vector2i> t_coordinates)
{
    m_gridCoordinates = t_coordinates;

    if (m_gridCoordinates.has_value())
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // Set the gobblet's world position to the grid coordinates
                if (m_gridCoordinates->x == i && m_gridCoordinates->y == j)
                {
                    auto tile = t_grid.getGridArray()[i][j];
                    m_shape.setPosition(tile.getCenter());
                }
            }
        }
    }
}

sf::Vector2f Gobblet::getPosition() const
{
    return m_shape.getPosition();
}

void Gobblet::setPosition(const sf::Vector2f& t_position)
{
    m_shape.setPosition(t_position);
}

sf::CircleShape Gobblet::getShape()
{
    return m_shape;
}

const sf::CircleShape& Gobblet::getShape() const
{
    return m_shape;
}

void Gobblet::deactivateClickedState()
{
    m_shape.setOutlineColor(sf::Color::Transparent);
    m_shape.setOutlineThickness(0);
}

void Gobblet::activateClickedState()
{
    m_shape.setOutlineColor(sf::Color::Blue);
    m_shape.setOutlineThickness(2);
}

void Gobblet::verifySize()
{
    if (m_size > 4)
    {
        m_size = 4;
    }
    else if (m_size < 1)
    {
        m_size = 1;
    }
}

void Gobblet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}

bool operator==(const Gobblet& t_lhs, const Gobblet& t_rhs)
{
    return t_lhs.m_gridCoordinates == t_rhs.m_gridCoordinates
        && t_lhs.m_size == t_rhs.m_size
        && t_lhs.m_color == t_rhs.m_color;
}

bool operator!=(const Gobblet& t_lhs, const Gobblet& t_rhs)
{
    return !(t_lhs == t_rhs);
}
