#pragma once
#include <SFML/Graphics.hpp>

#include "GobbletStack.hpp"

class Gobblet;

class Tile : public sf::Drawable
{
public:
    Tile();
    Tile(int row, int col, int t_tileSize);
    ~Tile();
    void Update(sf::RectangleShape t_mouse);
    sf::Vector2f GetGridCoordinate();
    sf::RectangleShape& GetShape();
    void SetPosition(int t_x, int t_y);

    sf::Vector2f getCenter() const;

    GobbletStack& getStack();
    const GobbletStack& getStack() const;
    
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    GobbletStack m_stack;
    
    sf::RectangleShape m_tileShape;
    sf::Vector2f m_gridCoordinate;
    float m_tileSize = 20;
};
