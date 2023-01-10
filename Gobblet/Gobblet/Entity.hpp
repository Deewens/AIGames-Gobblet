#pragma once

#include <SFML/Graphics/Color.hpp>

#include "PlayerType.hpp"

class Entity
{
public:
    Entity(sf::Color t_gobbletColor, PlayerAIType t_AIType);
    sf::Color getColor() const;
    bool canUseReserves() const;
    void setReserveUse(bool t_b);
    
    const PlayerAIType& getType() const;

    friend bool operator==(const Entity& t_lhs, const Entity& t_rhs);

    friend bool operator!=(const Entity& t_lhs, const Entity& t_rhs);

private:
    PlayerAIType m_type;
    
    sf::Color m_entityColor;
    bool m_canUseReserves;
};
