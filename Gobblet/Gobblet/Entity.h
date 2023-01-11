#pragma once

#include <SFML/Graphics.hpp>

#include "Gobblet.hpp"

class Entity
{
public:
    enum class Type
    {
        Min,
        Max
    };
    
    Entity(sf::Color t_gobbletColor, Type t_type, bool t_isAI, Grid& t_grid);
    /*Entity(const Entity& t_entity);

    ~Entity();*/

    sf::Color getColor() const;

    const Type& getType() const;
    bool isAI() const;
    
    bool canUseReserves() const;
    void setReserveUse(bool t_b);

    std::vector<Gobblet*>& getExternalGobblets();
    const std::vector<Gobblet*>& getExternalGobblets() const;
    
private:
    std::vector<Gobblet*> m_externalGobblets;
    
    sf::Color m_gobbletColor;
    Type m_type;
    bool m_isAI = false;
    
    bool m_canUseReserves;
};

