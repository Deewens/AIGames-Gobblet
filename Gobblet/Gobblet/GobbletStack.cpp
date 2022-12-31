#include "GobbletStack.hpp"

GobbletStack::GobbletStack()
{
}

void GobbletStack::add(const Gobblet& t_gobblet)
{
    m_stack.push(t_gobblet);

    m_stack.top().setPosition(m_position);
}

Gobblet& GobbletStack::top()
{
    return m_stack.top();
}

void GobbletStack::remove()
{
    m_stack.pop();
    
    m_stack.top().setPosition(m_position);
}

void GobbletStack::setPosition(const sf::Vector2f& t_position)
{
    m_position = t_position;

    if (!m_stack.empty())
        m_stack.top().setPosition(m_position);
}

void GobbletStack::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_stack.empty())
    {
        target.draw(m_stack.top(), states);
    }
}

