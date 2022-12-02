#pragma once
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include <iostream>
class Grid
{
public:
	Grid();
	~Grid();

	void Update();
	void Draw(sf::RenderWindow& t_window);
	void MouseEvents(sf::Event t_event);
	void MouseGridCoord();
private:
	Tile * m_gridArray[4][4];
	sf::Font m_arialBlackfont; // font used by message
	sf::Text m_gridCoordinate; // text used for message on screen
	sf::Text m_mouseCoordinate; // text used for message on screen

	//int m_baseX = (sf::VideoMode::getDesktopMode().width/2)/2;
	int m_baseX = 720;
	int m_baseY = 300;
	int m_tileSize = 100;

	//Mouse Position
	sf::Vector2u m_mousePositionGrid;
	sf::Vector2f m_mousePositionView;
};

