#include "GobbletStack.hpp"

#include <iostream>

GobbletStack::GobbletStack() = default;

GobbletStack::GobbletStack(const bool t_isExternalStack) : isExternalStack(t_isExternalStack)
{
}

void GobbletStack::add(Gobblet& t_gobblet)
{
    m_stack.push(std::ref(t_gobblet));
}

Gobblet& GobbletStack::top()
{
    return m_stack.top();
}

Gobblet& GobbletStack::pop()
{
    const auto removedGobblet = m_stack.top();
    m_stack.pop();
    return removedGobblet;
}

bool GobbletStack::isEmpty() const
{
    return m_stack.empty();
}

void GobbletStack::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_stack.empty()) return;

    target.draw(m_stack.top(), states);
}

std::stack<std::reference_wrapper<Gobblet>>& GobbletStack::stack()
{
    return m_stack;
}

bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return t_lhs.m_stack == t_rhs.m_stack;
}

bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return !(t_lhs == t_rhs);
}
