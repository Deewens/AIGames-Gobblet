#include "Grid.h"

Grid::Grid()
{
	int x = 0;
	int y = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j= 0; j < 4; j++)
		{
			m_gridArray[i][j] = new Tile();
			//m_gridArray[i][j]->GetShape().setPosition(m_gridArray[i][j]->GetShape().getPosition().x + 100, m_gridArray[i][j]->GetShape().getPosition().y);
			m_gridArray[i][j]->SetPosition(x,y);

			x += 50;
		}
		x = 0;
		y += 50;
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
}

void Grid::MouseGridCoord()
{

}
