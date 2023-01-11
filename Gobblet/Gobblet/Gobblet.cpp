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

const Gobblet* Gobblet::getParentGobblet() const
{
    return m_parentGobblet;
}

const Gobblet* Gobblet::getChildGobblet() const
{
    return m_childGobblet;
}

Gobblet* Gobblet::getParentGobblet()
{
    return m_parentGobblet;
}

Gobblet* Gobblet::getChildGobblet()
{
    return m_childGobblet;
}

const Tile* Gobblet::getTile() const
{
    return m_tile;
}

Tile* Gobblet::getTile()
{
    return m_tile;
}

void Gobblet::setParentGobblet(Gobblet* t_parent)
{
    m_parentGobblet = t_parent;
}

void Gobblet::setChildGobblet(Gobblet* t_child)
{
    m_childGobblet = t_child;
}

void Gobblet::setTile(Tile* t_tile)
{
    m_tile = t_tile;
}

bool Gobblet::gobbleUp(const Grid& t_grid, Gobblet& t_biggerGobblet)
{
    if (t_biggerGobblet.getSize() > m_size)
    {
        t_biggerGobblet.m_childGobblet = this;
        m_parentGobblet = &t_biggerGobblet;
        m_tile = t_biggerGobblet.m_tile;
        t_biggerGobblet.setGridCoordinates(t_grid, m_gridCoordinates);
        
        return true;
    }
    else
    {
        return false;
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

sf::CircleShape& Gobblet::getShape()
{
    return m_shape;
}

void Gobblet::deactivateClickedState()
{
    getShape().setOutlineColor(sf::Color::Transparent);
    getShape().setOutlineThickness(0);
}

void Gobblet::activateClickedState()
{
    getShape().setOutlineColor(sf::Color::Blue);
    getShape().setOutlineThickness(2);
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
