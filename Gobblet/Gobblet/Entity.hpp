#pragma once

#include <list>
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
    Entity(const sf::Color& t_gobbletColor, PlayerType t_type, bool t_isAI);
    std::list<std::shared_ptr<GobbletStack>>& initExternalStacks(const Grid& t_grid);
    
    const sf::Color& getColor() const;
    const PlayerType& getType() const;

    bool canUseReserves() const;
    void setReserveUse(bool t_b);

    const std::list<Gobblet>& getGobblets() const;
    const std::list<std::shared_ptr<GobbletStack>>& getExternalStacks() const;

    bool isAI() const;

    friend bool operator==(const Entity& t_lhs, const Entity& t_rhs);

    friend bool operator!=(const Entity& t_lhs, const Entity& t_rhs);
    
private:
    std::list<Gobblet> m_gobblets;
    
    std::list<std::shared_ptr<GobbletStack>> m_externalStacks;
    
    const sf::Color m_gobbletColor;
    const PlayerType m_type;
    const bool m_isAI = false;

    bool m_canUseReserves;
};
