#pragma once
#include "SFML/Graphics.hpp"

class Entity
{
public:
    Entity(sf::Color t_entityColor, bool t_isAI);
    sf::Color GetColor();
    bool CanUseReserves();
    void SetReserveUse(bool t_b);
private:
    sf::Color m_entityColor;
    bool m_isAi;
    bool m_canUseReserves;
};

