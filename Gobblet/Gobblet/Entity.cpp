#include "Entity.h"

#include "Grid.h"

Entity::Entity(const sf::Color t_gobbletColor, const Type t_type, const bool t_isAI, Grid& t_grid):
    m_gobbletColor(t_gobbletColor),
    m_type(t_type),
    m_isAI(t_isAI)
{
    m_canUseReserves = false;

    // Initialise the board, create external stacks around the grid
    const sf::Vector2f gridPosition = t_grid.getPosition();
    const sf::Vector2f gridSize = t_grid.getSize();

    float yPosition = gridPosition.y;
    for (int i = 0; i < 3; i++)
    {
        yPosition = yPosition + 50;
        for (int size = 1; size <= 4; size++)
        {
            Gobblet* newGobblet = new Gobblet(t_gobbletColor, size);
            if (m_gobbletColor == sf::Color::Black)
            {
                newGobblet->setPosition({gridPosition.x - 50, yPosition}); // 50px - left side of the grid
            }
            else
            {
                newGobblet->setPosition({gridPosition.x + (gridSize.x + 50), yPosition});
                // 50px - right side of the grid
            }

            m_externalGobblets.push_back(newGobblet);
            if (m_externalGobblets.size() > 1)
            {
                m_externalGobblets[m_externalGobblets.size() - 2]->gobbleUp(t_grid, *m_externalGobblets.back());
            }
        }
    }
}



/*Entity::Entity(const Entity& t_entity) :
    m_externalGobblets(t_entity.m_externalGobblets),
    m_gobbletColor(t_entity.m_gobbletColor),
    m_type(t_entity.m_type),
    m_isAI(t_entity.m_isAI),
    m_canUseReserves(t_entity.m_canUseReserves)
{
    /*for (const auto& ptr : t_entity.m_externalGobblets)
    {
        if (ptr) m_externalGobblets.push_back(std::make_unique<Gobblet>(*ptr));
    }#1#
}*/

sf::Color Entity::getColor() const
{
    return m_gobbletColor;
}

const Entity::Type& Entity::getType() const
{
    return m_type;
}

bool Entity::isAI() const
{
    return m_isAI;
}

bool Entity::canUseReserves() const
{
    return m_canUseReserves;
}

void Entity::setReserveUse(const bool t_b)
{
    m_canUseReserves = t_b;
}

std::vector<Gobblet*>& Entity::getExternalGobblets()
{
    return m_externalGobblets;
}

const std::vector<Gobblet*>& Entity::getExternalGobblets() const
{
    return m_externalGobblets;
}
