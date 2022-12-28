#include "Gobblet.hpp"

#include "Grid.h"

Gobblet::Gobblet(const Color& t_color, const int t_size, Grid& t_grid) :
    m_grid(t_grid),
    m_size(t_size),
    m_shape(static_cast<float>(t_size) * m_sizeFactor)
{
    verifySize();

    if (t_color == Color::White)
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

void Gobblet::setGridCoordinates(std::optional<sf::Vector2i> t_coordinates)
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
                    auto tile = m_grid.getGridArray()[i][j];
                    m_shape.setPosition(tile.getCenter());
                }
            }
        }
    }
}

bool Gobblet::gobbleUp(const std::shared_ptr<Gobblet>& t_biggerGobblet)
{
    if (t_biggerGobblet->getSize() > m_size)
    {
        t_biggerGobblet->setGridCoordinates(m_gridCoordinates);

        m_parentGobblet = t_biggerGobblet;
        t_biggerGobblet->m_childGobblet = shared_from_this();

        return true;
    }

    return false;
}

std::shared_ptr<Gobblet> Gobblet::getParentGobblet()
{
    return m_parentGobblet;
}

std::shared_ptr<Gobblet> Gobblet::getChildGobblet()
{
    return m_childGobblet;
}

sf::Vector2f Gobblet::getPosition() const
{
    return m_shape.getPosition();
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
    if (m_gridCoordinates.has_value() || m_parentGobblet == nullptr)
    {
        target.draw(m_shape, states);
    }
}
