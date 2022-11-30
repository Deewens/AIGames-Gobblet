#pragma once
#include <SFML/Graphics.hpp>
class Tile
{
public:
	Tile();
	~Tile();
	sf::RectangleShape& GetShape();
	void SetPosition(int t_x, int t_y);
private:
	sf::RectangleShape m_tileShape;
	float m_tileSize;
};

