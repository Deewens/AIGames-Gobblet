#include "Grid.h"

Grid::Grid()
{
	if (!m_arialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	int x = m_baseX;
	int y = m_baseY;

	//Debug mouse coordinates
	m_mouseCoordinateText.setFont(m_arialBlackfont);
	m_mouseCoordinateText.setPosition(10.0f, 40.0f);
	m_mouseCoordinateText.setCharacterSize(20U);
	m_mouseCoordinateText.setFillColor(sf::Color::White);
	m_mouseCoordinateText.setOutlineThickness(3.0f);
	m_mouseCoordinateText.setString("Mouse Position: blank, blank");

	m_gridCoordinateText.setFont(m_arialBlackfont);
	m_gridCoordinateText.setPosition(10.0f, 80.0f);
	m_gridCoordinateText.setCharacterSize(20U);
	m_gridCoordinateText.setFillColor(sf::Color::White);
	m_gridCoordinateText.setOutlineThickness(3.0f);
	m_gridCoordinateText.setString("Grid Position: blank, blank");

	//Sets up an invisible bounding box for where the mouse is
	m_invisibleMouse.setFillColor(sf::Color::Blue);
	m_invisibleMouse.setSize(sf::Vector2f(1,1));
	m_invisibleMouse.setOrigin(0.5f,0.5f);

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
	GridCoord();
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
	GridCoord();
	t_window.draw(m_mouseCoordinateText);
	t_window.draw(m_gridCoordinateText);
	t_window.draw(m_invisibleMouse);
}

void Grid::MouseEvents(sf::Event t_event)
{

	m_mouseCoordinateText.setString("Mouse Position: " + std::to_string(m_mousePositionView.x) + " | " + std::to_string(m_mousePositionView.y));
}

void Grid::GridCoord()
{
	m_invisibleMouse.setPosition(m_mousePositionView);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//Displays the grid co-ordinate relative to where the mouse is
			if (m_invisibleMouse.getGlobalBounds().intersects(m_gridArray[i][j]->GetShape().getGlobalBounds()))
			{
				sf::Vector2u g = static_cast<sf::Vector2u> (m_gridArray[i][j]->GetGridCoordinate());
				m_gridCoordinateText.setString("Grid: " + std::to_string(g.x) + ", " + std::to_string(g.y));
			}

		}
	}
}
