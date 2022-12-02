#include "Gobblet.hpp"

Gobblet::Gobblet(const Color& t_color, const int t_size) :
    m_size(t_size),
    m_parentGobblet(nullptr),
    m_childGobblet(nullptr),
    m_shape(static_cast<float>(t_size))
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
    
    m_shape.setOrigin(0.5f, 0.5f);
    m_shape.setFillColor(m_color);
}

int Gobblet::getSize() const
{
    return m_size;
}

sf::Vector2i Gobblet::getGridCoordinates() const
{
    return *m_gridCoordinates;
}

void Gobblet::setGridCoordinates(const sf::Vector2i& coordinates)
{
    m_gridCoordinates = std::make_unique<sf::Vector2i>(coordinates);
}

bool Gobblet::gobbleUp(Gobblet& t_biggerGobblet)
{
    if (t_biggerGobblet.m_size > m_size)
    {
        m_parentGobblet = std::make_shared<Gobblet>(t_biggerGobblet);
        t_biggerGobblet.m_childGobblet = std::make_shared<Gobblet>(t_biggerGobblet);
        return true;
    }

    return false;
}

std::shared_ptr<Gobblet> Gobblet::getParentGobblet()
{
    return m_parentGobblet;
}

std::shared_ptr<Gobblet> Gobblet::getGobbledUpGobblet()
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
}
