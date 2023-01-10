#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "PlayerType.hpp"

class GobbletStack;
class Gobblet;
class Grid;

class Entity
{
public:
    Entity();
    Entity(const sf::Color& t_gobbletColor, PlayerAIType t_AIType);
    void initExternalStacks(const Grid& t_grid);
    
    const sf::Color& getColor() const;
    const PlayerAIType& getType() const;

    bool canUseReserves() const;
    void setReserveUse(bool t_b);

    const std::vector<GobbletStack>& getExternalStacks() const;

    friend bool operator==(const Entity& t_lhs, const Entity& t_rhs);

    friend bool operator!=(const Entity& t_lhs, const Entity& t_rhs);

private:
    std::vector<Gobblet> m_gobblets;
    std::vector<GobbletStack> m_externalStacks;
    
    const sf::Color m_gobbletColor;
    const PlayerAIType m_type;

    bool m_canUseReserves;
};
