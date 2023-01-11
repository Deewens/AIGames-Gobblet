#include "GobbletStack.hpp"

#include <iostream>

#include "Grid.h"

GobbletStack::GobbletStack(const bool t_isExternalStack) :
    isExternalStack(t_isExternalStack)
{
}

void GobbletStack::add(const Gobblet& t_gobblet)
{
    m_stack.push_back(t_gobblet);

    std::sort(m_stack.begin(), m_stack.end(), [](const Gobblet& t_gobblet1, const Gobblet& t_gobblet2)
    {
        return t_gobblet1.getSize() < t_gobblet2.getSize();
    });
}

Gobblet& GobbletStack::top()
{
    return m_stack.back();
}

const Gobblet& GobbletStack::top() const
{
    return m_stack.back();
}

Gobblet GobbletStack::pop()
{
    auto removedGobblet = m_stack.back();
    m_stack.pop_back();

    return removedGobblet;
}

bool GobbletStack::isEmpty() const
{
    return m_stack.empty();
}

void GobbletStack::setPosition(sf::Vector2f t_position)
{
    m_position = t_position;
    setGobbletsPosition(m_position);
}

void GobbletStack::setGridPosition(const Grid& t_grid, const std::optional<sf::Vector2i>& t_gridPosition)
{
    m_gridPosition = t_gridPosition;

    if (t_gridPosition)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // Set the gobblet's world position to the grid coordinates
                if (t_gridPosition->x == i && t_gridPosition->y == j)
                {
                    auto tile = t_grid.getGridArray()[i][j];
                    setPosition(tile.getCenter());
                }
            }
        }

        setGobbletsGridPosition(t_grid, t_gridPosition);
    }
}

sf::Vector2f GobbletStack::getPosition()
{
    return m_position;
}

const sf::Vector2f& GobbletStack::getPosition() const
{
    return m_position;
}

void GobbletStack::setGobbletsPosition(const sf::Vector2f t_position)
{
    for (Gobblet& gobblet : m_stack)
    {
        gobblet.setPosition(t_position);
    }

    m_position = t_position;
}

void GobbletStack::setGobbletsGridPosition(const Grid& t_grid, const std::optional<sf::Vector2i>& t_gridPosition)
{
    for (Gobblet& gobblet : m_stack)
    {
        gobblet.setGridCoordinates(t_grid, t_gridPosition);
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

std::vector<Gobblet> GobbletStack::container()
{
    return m_stack;
}

const std::vector<Gobblet>& GobbletStack::container() const
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
