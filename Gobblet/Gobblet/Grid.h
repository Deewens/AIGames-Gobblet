#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include "Tile.h"

class Grid : public sf::Drawable
{
public:
    /**
     * \brief Size of the grid in tiles (horizontal and vertical)
     */
    constexpr static int Size = 4;
    
    Grid();
    ~Grid();

    void update(const sf::RenderWindow& t_window);
    void gridCoord();

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    
    std::array<std::array<Tile, 4>, 4>& getGridArray();

    sf::FloatRect getBounds() const;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
private:

    std::array<std::array<Tile, 4>, 4> m_gridArray;
    sf::Font m_arialBlackfont; // font used by message
	sf::Text m_gridCoordinateText; // text used for message on screen
	sf::Text m_mouseCoordinateText; // text used for message on screen

    //int m_baseX = (sf::VideoMode::getDesktopMode().width/2)/4;
	//int m_baseY = (sf::VideoMode::getDesktopMode().height/2)/4;
    int m_baseX = 300;
    int m_baseY = 300;
    //int m_tileSize = 100;
    int m_tileSize = 50; // Use of 50 for testing because 100 is too big for 1080p screen

    //Mouse Position
    sf::Vector2u m_mousePositionGrid;
    sf::Vector2f m_mousePositionView;

    sf::RectangleShape m_invisibleMouse;

    sf::FloatRect m_bounds;
};
