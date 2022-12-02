#include "Grid.h"

Grid::Grid()
{
	if (!m_arialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	int x = m_baseX;
	int y = m_baseY;


	m_mouseCoordinate.setFont(m_arialBlackfont);
	m_mouseCoordinate.setPosition(10.0f, 40.0f);
	m_mouseCoordinate.setCharacterSize(20U);
	m_mouseCoordinate.setFillColor(sf::Color::White);
	m_mouseCoordinate.setOutlineThickness(3.0f);
	m_mouseCoordinate.setString("Mouse Position: blank, blank");

	for (int i = 0; i < 4; i++)
	{
		for (int j= 0; j < 4; j++)
		{
			m_gridArray[i][j] = new Tile(i, j, m_tileSize);
			m_gridArray[i][j]->SetPosition(x,y);

			x += m_tileSize;
		}
		x = m_baseX;
		y += m_tileSize;
	}
}

Grid::~Grid()
{

}

void Grid::Update()
{

}

void Grid::Draw(sf::RenderWindow& t_window)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t_window.draw(m_gridArray[i][j]->GetShape());
			
		}
	}

	m_mousePositionView = t_window.mapPixelToCoords(sf::Mouse::getPosition(t_window));
	MouseGridCoord();
	t_window.draw(m_mouseCoordinate);
}

void Grid::MouseEvents(sf::Event t_event)
{
	m_mouseCoordinate.setString("Mouse Position: " + std::to_string(m_mousePositionView.x) + " | " + std::to_string(m_mousePositionView.y));
}

void Grid::MouseGridCoord()
{

}
