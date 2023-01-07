#pragma once

#include <SFML/Graphics/Color.hpp>

#include "PlayerType.hpp"

class Entity
{
public:
    Entity(sf::Color t_entityColor, bool t_isAI);
    sf::Color GetColor();
    bool CanUseReserves();
    void SetReserveUse(bool t_b);
    
    const PlayerAIType& getType() const;

    friend bool operator==(const Entity& t_lhs, const Entity& t_rhs);

    friend bool operator!=(const Entity& t_lhs, const Entity& t_rhs);

private:
    PlayerAIType m_type;
    
    sf::Color m_entityColor;
    bool m_isAi;
    bool m_canUseReserves;
};
