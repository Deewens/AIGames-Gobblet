#include "Tile.h"

Tile::Tile()
{
	m_tileSize = 50;
	m_tileShape.setFillColor(sf::Color::White);
	m_tileShape.setOutlineColor(sf::Color::Red);
	m_tileShape.setOutlineThickness(2);
	m_tileShape.setSize(sf::Vector2f( m_tileSize, m_tileSize));
}

Tile::~Tile()
{
}

sf::RectangleShape& Tile::GetShape()
{
	return m_tileShape;
}

void Tile::SetPosition(int t_x, int t_y)
{
	m_tileShape.setPosition(sf::Vector2f(t_x, t_y));
}

