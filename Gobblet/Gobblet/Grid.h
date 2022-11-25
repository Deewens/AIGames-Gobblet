#pragma once
#include "SFML/Graphics.hpp"
#include "Tile.h"
class Grid
{
public:
	Grid();
	~Grid();

	void Update();
	void Draw(sf::RenderWindow& t_window);
	void MouseGridCoord();
private:
	Tile m_gridArray[4][4];
	sf::Font m_arialBlackfont; // font used by message
	sf::Text m_gridCoordinate; // text used for message on screen
};

