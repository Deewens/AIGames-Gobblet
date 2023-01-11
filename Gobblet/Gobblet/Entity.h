#pragma once

#include <SFML/Graphics.hpp>

#include "Gobblet.hpp"

class GobbletStack;

class Entity
{
public:
    enum class Type
    {
        Min,
        Max
    };
    
    Entity(sf::Color t_gobbletColor, Type t_type, bool t_isAI, const Grid& t_grid);
    /*Entity(const Entity& t_entity);

    ~Entity();*/

    sf::Color getColor() const;

    const Type& getType() const;
    bool isAI() const;
    
    bool canUseReserves() const;
    void setReserveUse(bool t_b);
    
    const std::vector<GobbletStack>& getExternalStacks() const;
    std::vector<GobbletStack>& getExternalStacks();

    
    
private:
    std::vector<GobbletStack> m_externalStacks;
    
    sf::Color m_gobbletColor;
    Type m_type;
    bool m_isAI = false;
    
    bool m_canUseReserves;
};
