#include "GobbletStack.hpp"

#include <iostream>

GobbletStack::GobbletStack() = default;

GobbletStack::GobbletStack(const bool t_isExternalStack) : isExternalStack(t_isExternalStack)
{
}

void GobbletStack::add(std::reference_wrapper<Gobblet> t_gobblet)
{
    m_stack.push_back(t_gobblet);
    t_gobblet.get().getStack() = shared_from_this();
    
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
    
    removedGobblet.get().getStack().reset();
    
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
    //if (m_stack.empty()) return;

    for (auto& gobblet : m_stack)
    {
        target.draw(gobblet, states);
    }
}

std::list<std::reference_wrapper<Gobblet>>& GobbletStack::stack()
{
    return m_stack;
}

std::list<std::reference_wrapper<Gobblet>> GobbletStack::stack() const
{
    return m_stack;
}

bool GobbletStack::isClicked() const
{
    return m_isClicked;
}

void GobbletStack::setClicked(const bool t_isClicked)
{
    m_isClicked = t_isClicked;
}

bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return t_lhs.m_stack == t_rhs.m_stack
        && t_lhs.isExternalStack == t_rhs.isExternalStack
        && t_lhs.m_isClicked == t_rhs.m_isClicked;
}

bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return !(t_lhs == t_rhs);
}
