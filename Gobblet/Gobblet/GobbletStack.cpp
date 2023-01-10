#include "GobbletStack.hpp"

#include <iostream>

GobbletStack::GobbletStack() = default;

GobbletStack::GobbletStack(const bool t_isExternalStack) : isExternalStack(t_isExternalStack)
{
}

void GobbletStack::add(Gobblet& t_gobblet)
{
    m_stack.push_back(std::ref(t_gobblet));

    m_stack.sort([](const Gobblet& t_gobblet1, const Gobblet& t_gobblet2)
    {
        return t_gobblet1.getSize() < t_gobblet2.getSize();
    });
}

Gobblet& GobbletStack::top() const
{
    return m_stack.back();
}

Gobblet& GobbletStack::pop()
{
    const auto removedGobblet = m_stack.back();
    m_stack.pop_back();
    return removedGobblet;
}

bool GobbletStack::isEmpty() const
{
    return m_stack.empty();
}

void GobbletStack::setPosition(const sf::Vector2f t_position) const
{
    for (auto gobblet : m_stack)
    {
        gobblet.get().setPosition(t_position);
    }
}

void GobbletStack::setGridPosition(Grid& t_grid, const sf::Vector2i t_gridPosition) const
{
    for (auto gobblet : m_stack)
    {
        gobblet.get().setGridCoordinates(t_gridPosition, t_grid);
    }
}

void GobbletStack::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_stack.empty()) return;

    target.draw(m_stack.back(), states);
}

std::list<std::reference_wrapper<Gobblet>>& GobbletStack::stack()
{
    return m_stack;
}

bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return t_lhs.m_stack == t_rhs.m_stack
        && t_lhs.isExternalStack == t_rhs.isExternalStack
        && t_lhs.isClicked == t_rhs.isClicked;
}

bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return !(t_lhs == t_rhs);
}
