#pragma once

#include "GobbletStack.hpp"
#include "Grid.h"

class Board : public sf::Drawable
{
public:
    explicit Board(sf::RenderWindow& t_window);

    void update(sf::Time t_deltaTime);
    
    void processMouse(sf::Event t_event);
    
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
private:
    sf::RenderWindow& m_window;
    
    Grid m_grid;

    std::vector<GobbletStack> m_gobbletStacks;
};
