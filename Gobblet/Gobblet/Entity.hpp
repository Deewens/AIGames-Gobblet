#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics/Color.hpp>

#include "PlayerType.hpp"

class Gobblet;
class Grid;
class GobbletStack;

class Entity
{
public:
    Entity(Grid& t_grid, const PlayerType& t_playerType, const sf::Color& t_gobbletColor);

    sf::Color getColor();
    bool canUseReserves();
    void setReserveUse(bool t_b);

    void setGobblets(const std::vector<Gobblet>& t_gobblets);
    Gobblet& addGobblet(const Gobblet& t_gobblet);
    std::vector<Gobblet>& getGobblets();
    
    const PlayerType& getType() const;

    friend bool operator==(const Entity& t_lhs, const Entity& t_rhs);

    friend bool operator!=(const Entity& t_lhs, const Entity& t_rhs);

private:
    std::vector<Gobblet> m_gobblets;
    PlayerType m_type;

    sf::Color m_entityColor;
    bool m_canUseReserves;
};
