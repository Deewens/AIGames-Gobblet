#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Tile
{
public:
	Tile();
	Tile(int row, int col, int t_tileSize);
	~Tile();
	sf::RectangleShape& GetShape();
	void SetPosition(int t_x, int t_y);
private:
	sf::RectangleShape m_tileShape;
	float m_tileSize = 20;
	sf::Vector2f m_gridCoordinate;
};

