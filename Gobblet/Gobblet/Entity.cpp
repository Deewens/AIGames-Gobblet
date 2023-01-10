#include "Entity.hpp"

#include "GobbletStack.hpp"
#include "Grid.h"

Entity::Entity(const sf::Color& t_gobbletColor, PlayerAIType t_AIType) :
    m_gobbletColor(t_gobbletColor),
    m_type(t_AIType)
{
    m_canUseReserves = false;
}

void Entity::initExternalStacks(const Grid& t_grid)
{
    // Initialise the board, create external stacks around the grid
    const sf::Vector2f gridPosition = t_grid.getPosition();
    const sf::Vector2f gridSize = t_grid.getSize();

    float yPosition = gridPosition.y;

    for (int i = 0; i < 3; i++)
    {
        GobbletStack stack = GobbletStack(true);

        yPosition = yPosition + 50;
        for (int size = 1; size <= 4; size++)
        {
            m_gobblets.emplace_back(m_gobbletColor, size);
            stack.add(std::ref(m_gobblets.back()));
        }

        if (m_gobbletColor == sf::Color::Black)
        {
            stack.setPosition({gridPosition.x - 50, yPosition}); // 50px - left side of the grid
        }
        else
        {
            stack.setPosition({gridPosition.x + (gridSize.x + 50), yPosition}); // 50px - right side of the grid
        }

        m_externalStacks.push_back(stack);
    }
}

const sf::Color& Entity::getColor() const
{
    return m_gobbletColor;
}

const PlayerAIType& Entity::getType() const
{
    return m_type;
}

bool Entity::canUseReserves() const
{
    return m_canUseReserves;
}

void Entity::setReserveUse(bool t_b)
{
    m_canUseReserves = t_b;
}

const std::vector<Gobblet>& Entity::getGobblets() const
{
    return m_gobblets;
}

const std::vector<GobbletStack>& Entity::getExternalStacks() const
{
    return m_externalStacks;
}

bool operator==(const Entity& t_lhs, const Entity& t_rhs)
{
    return t_lhs.m_type == t_rhs.m_type
        && t_lhs.m_gobbletColor == t_rhs.m_gobbletColor;
}

bool operator!=(const Entity& t_lhs, const Entity& t_rhs)
{
    return !(t_lhs == t_rhs);
}
