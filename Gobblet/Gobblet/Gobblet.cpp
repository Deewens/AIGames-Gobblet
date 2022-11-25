#include "Gobblet.hpp"

Gobblet::Gobblet(const sf::Color& t_color, int t_size)
    : m_color(t_color),
      m_size(t_size)

{
    verifySize();
}

int Gobblet::getSize() const
{
    return m_size;
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

bool Gobblet::gobbleUp(const std::shared_ptr<Gobblet>& t_biggerGobblet)
{
    if (t_biggerGobblet->m_size > m_size)
    {
        m_parentGobblet = t_biggerGobblet;
        t_biggerGobblet->m_childGobblet = t_biggerGobblet;
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
