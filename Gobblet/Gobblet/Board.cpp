#include "Board.hpp"

#include "Gobblet.hpp"

Board::Board(sf::RenderWindow& t_window) :
    m_window(t_window), m_mousePosition(0, 0, 1, 1),
    m_maxPlayer(sf::Color::Black, Entity::Type::Max, false, m_grid),
    m_minPlayer(sf::Color::White, Entity::Type::Min, true, m_grid)
{
    m_gobblets.insert(m_gobblets.end(), m_minPlayer.getExternalGobblets().begin(), m_minPlayer.getExternalGobblets().end());
    m_gobblets.insert(m_gobblets.end(), m_minPlayer.getExternalGobblets().begin(), m_minPlayer.getExternalGobblets().end());
    
    m_currentPlayer = &m_maxPlayer;
    m_sameActionCount = 0;
}

void Board::update(sf::Time t_deltaTime)
{
    m_grid.update(m_window);

    const sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    m_mousePosition.left = mousePosition.x;
    m_mousePosition.top = mousePosition.y;
}

void Board::deactivateClickedGobblet()
{
    if (!m_clickedGobblet.expired())
    {
        m_clickedGobblet.lock()->deactivateClickedState();
        m_clickedGobblet.reset();
    }
}

bool Board::chooseGobblet()
{
    if (m_gobbletActionState == ActionState::ChooseGobblet)
    {
        if (!m_clickedGobblet.expired())
        {
            deactivateClickedGobblet();
        }
        
        for (auto const& gobblet : m_currentPlayer->getExternalGobblets())
        {
            if (gobblet->getParentGobblet().expired() && gobblet->getShape().getGlobalBounds().intersects(m_mousePosition))
            {
                m_clickedGobblet = gobblet;
                m_clickedGobblet.lock()->activateClickedState();
                m_gobbletActionState = ActionState::PlaceGobblet;
                
                return true;
            }
        }

        for (auto const& gobblet : m_gobblets)
        {
            if (gobblet->getParentGobblet().expired() && gobblet->getColor() == m_currentPlayer->getColor() && gobblet->getShape().getGlobalBounds().intersects(m_mousePosition))
            {
                m_clickedGobblet = gobblet;
                m_clickedGobblet.lock()->activateClickedState();
                m_gobbletActionState = ActionState::PlaceBoardGobblet;
                
                return true;
            }
        }
    }
    
    return false;
}

void Board::switchCurrentPlayer()
{
    if (m_currentPlayer->getType() == Entity::Type::Max)
    {
        m_currentPlayer = &m_minPlayer;
    }
    else
    {
        m_currentPlayer = &m_maxPlayer;
    }
}

void Board::removeClickedGobbletFromReserve() const
{
    // If external stack
    if (!m_clickedGobblet.expired() && !m_clickedGobblet.lock()->getGridCoordinates().has_value())
    {
        m_currentPlayer->getExternalGobblets().erase(std::remove(m_currentPlayer->getExternalGobblets().begin(), m_currentPlayer->getExternalGobblets().end(), m_clickedGobblet.lock()), m_currentPlayer->getExternalGobblets().end());
    }
}

bool Board::placeGobblet()
{
    if (m_gobbletActionState == ActionState::PlaceGobblet || m_gobbletActionState == ActionState::PlaceBoardGobblet)
    {
        // If mouse clicked on a tile of the grid, move the top gobblet from the active gobblet stack to the tile
        if (!m_clickedGobblet.expired())
        {
            // Mouse is on the grid
            if (m_grid.getBounds().intersects(m_mousePosition))
            {
                auto& gridArray = m_grid.getGridArray();
                for (size_t y = 0; y < gridArray.size(); y++)
                {
                    for (size_t x = 0; x < gridArray[y].size(); x++)
                    {
                        // Detect clicked tile on the grid
                        auto& tile = gridArray[y][x];
                        if (tile.GetShape().getGlobalBounds().intersects(m_mousePosition))
                        {
                            // If there is not gobblet stack on this tile, place it
                            if (tile.activeGobblet.expired())
                            {
                                removeClickedGobbletFromReserve();
                                    
                                tile.activeGobblet = m_clickedGobblet.lock();

                                if (!tile.activeGobblet.lock()->getChildGobblet().expired())
                                {
                                    tile.activeGobblet.lock()->getChildGobblet().lock()->setParentGobblet(nullptr);
                                }

                                if (m_clickedGobblet.lock()->getGridCoordinates().has_value())
                                {
                                    if (!m_clickedGobblet.lock()->getChildGobblet().expired())
                                    {
                                        m_clickedGobblet.lock()->getTile()->activeGobblet.reset();
                                    }
                                    else
                                    {
                                        m_clickedGobblet.lock()->getTile()->activeGobblet = m_clickedGobblet.lock()->getChildGobblet();
                                    }
                                }
                                
                                
                                tile.activeGobblet.lock()->setParentGobblet(nullptr);
                                tile.activeGobblet.lock()->setChildGobblet(nullptr);
                                tile.activeGobblet.lock()->setTile(&tile);
                                
                                tile.activeGobblet.lock()->setGridCoordinates(m_grid, sf::Vector2i(static_cast<int>(y), static_cast<int>(x)));

                                deactivateClickedGobblet();
                            }
                            else // If a gobblet already exist on this tile, gobble it up
                            {
                                if (m_gobbletActionState == ActionState::PlaceGobblet)
                                {
                                    if (m_currentPlayer->canUseReserves())
                                    {
                                        if (m_clickedGobblet.lock()->getSize() > tile.activeGobblet.lock()->getSize())
                                        {
                                            const bool hasGobbledUp = tile.activeGobblet.lock()->gobbleUp(m_grid, m_clickedGobblet.lock());
                                            if (hasGobbledUp)
                                            {
                                                removeClickedGobbletFromReserve();
                                                
                                                if (m_clickedGobblet.lock()->getGridCoordinates().has_value())
                                                {
                                                    if (!m_clickedGobblet.lock()->getChildGobblet().expired())
                                                    {
                                                        m_clickedGobblet.lock()->getTile()->activeGobblet.reset();
                                                    }
                                                    else
                                                    {
                                                        m_clickedGobblet.lock()->getTile()->activeGobblet = m_clickedGobblet.lock()->getChildGobblet();
                                                    }
                                                }
                                                
                                                tile.activeGobblet = m_clickedGobblet;
                                                deactivateClickedGobblet();

                                                m_currentPlayer->setReserveUse(false);
                                            }
                                        }
                                        else //Guard that breaks out and returns to the loop if both gobblets are of the same size
                                        {
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    if (m_clickedGobblet.lock()->getSize() > tile.activeGobblet.lock()->getSize())
                                    {
                                        const bool hasGobbledUp = tile.activeGobblet.lock()->gobbleUp(m_grid, m_clickedGobblet.lock());
                                        if (hasGobbledUp)
                                        {
                                            removeClickedGobbletFromReserve();

                                            if (m_clickedGobblet.lock()->getGridCoordinates().has_value())
                                            {
                                                if (!m_clickedGobblet.lock()->getChildGobblet().expired())
                                                {
                                                    m_clickedGobblet.lock()->getTile()->activeGobblet.reset();
                                                }
                                                else
                                                {
                                                    m_clickedGobblet.lock()->getTile()->activeGobblet = m_clickedGobblet.lock()->getChildGobblet();
                                                }
                                            }
                                            
                                            tile.activeGobblet = m_clickedGobblet;
                                            deactivateClickedGobblet();
                                        }
                                    }
                                    else
                                    //Guard that breaks out and returns to the loop if both gobblets are of the same size
                                    {
                                        break;
                                    }
                                }
                            }

                            m_gobbletActionState = ActionState::ChooseGobblet;

                            switchCurrentPlayer();

                            if (CheckWinCondition(m_currentPlayer->getColor()))
                            {
                                std::cout << (m_currentPlayer->getType() == Entity::Type::Max ? "Max wins" : "Min wins") << std::endl;
                            }

                            return true;
                        }
                    }
                }
            }
            else if (m_gobbletActionState == ActionState::PlaceGobblet)
            {

                deactivateClickedGobblet();

                m_gobbletActionState = ActionState::ChooseGobblet;
            }
        }
    }
    return false;
}

void Board::processMouse(sf::Event t_event)
{
    if (t_event.type == sf::Event::MouseButtonPressed)
    {
        if (t_event.mouseButton.button == sf::Mouse::Left)
        {
            // If method is success, break from the current control flow. placeGobblet will be called on next click
            if (chooseGobblet()) return;

            placeGobblet();
        }
        else if (t_event.mouseButton.button == sf::Mouse::Right)
        {
            if (!m_clickedGobblet.expired())
            {
                m_clickedGobblet.lock()->deactivateClickedState();
                m_clickedGobblet.reset();
                
                m_gobbletActionState = ActionState::ChooseGobblet;
            }
        }
    }
}


bool Board::CheckWinCondition(sf::Color t_color)
{
    /*auto& gridArray = m_grid.getGridArray();

    //Checks every row
    for (size_t y = 0; y < gridArray.size(); y++)
    {
        sf::Color rowOfColors[4] = {sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color::Blue};
        sf::Color colOfColors[4] = {sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color::Blue};


        for (size_t x = 0; x < gridArray[y].size(); x++)
        {
            //checks all rows
            auto& tile = gridArray[y][x];

            //Checks if there is a gobblet stack on the tile
            //Checks for if the tile's gobblet is the same colour as the player's
            if (!tile.gobbletStack.expired() && tile.gobbletStack.lock().get()->top().getShape().getFillColor() ==
                t_color)
            {
                rowOfColors[x] = t_color;
            }

            //Checks if all of in the array is true
            if (std::all_of(
                std::begin(rowOfColors),
                std::end(rowOfColors),
                [t_color](sf::Color i)
                {
                    return i == t_color;
                }
            ))
            {
                std::cout << "4 in a row" << std::endl;
                return true;
            }


            //Checks all columns
            auto& tileB = gridArray[x][y];


            if (!tileB.gobbletStack.expired() && tileB.gobbletStack.lock().get()->top().getShape().getFillColor() ==
                t_color)
            {
                colOfColors[x] = t_color;
            }

            //Checks if all of in the array is true
            if (std::all_of(
                std::begin(colOfColors),
                std::end(colOfColors),
                [t_color](sf::Color i)
                {
                    return i == t_color;
                }
            ))
            {
                std::cout << "4 in a col" << std::endl;
                return true;
            }
        }

        //Checks if either players can gobble using their reserves if there are 3 gobblets of the same colour in a row
        //if (!m_player.CanUseReserves())
        //{
        //    m_player.SetReserveUse(CheckReserveUsage(rowOfColors, colOfColors, sf::Color::Black));
        //}

        //if (!m_NPCPlayer.CanUseReserves())
        //{
        //    m_NPCPlayer.SetReserveUse(CheckReserveUsage(rowOfColors, colOfColors, sf::Color::White));
        //}

        if (CheckReserveUsage(rowOfColors, colOfColors, sf::Color::Black))
        {
            if (!m_player.canUseReserves())
            {
                m_player.setReserveUse(true);
            }
        }


        if (CheckReserveUsage(rowOfColors, colOfColors, sf::Color::White))
        {
            if (!m_NPCPlayer.canUseReserves())
            {
                m_NPCPlayer.setReserveUse(true);
            }
        }
    }


    sf::Color diagonalOfColorsA[4] = {sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color::Blue};
    sf::Color diagonalOfColorsB[4] = {sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color::Blue};

    //Checks for top left to bottom right to see if there's gobblets
    int count = 3;

    /// <summary>
    /// Checks the diagonals if there is 4 in a row
    /// </summary>
    /// <param name="t_color"></param>
    /// <returns></returns>
    for (int i = 0; i < 4; i++)
    {
        auto& tile = gridArray[i][i];

        if (!tile.gobbletStack.expired() && tile.gobbletStack.lock().get()->top().getShape().getFillColor() == t_color)
        {
            diagonalOfColorsA[i] = t_color;
        }

        auto& tileB = gridArray[count][i];

        if (!tileB.gobbletStack.expired() && tileB.gobbletStack.lock().get()->top().getShape().getFillColor() ==
            t_color)
        {
            diagonalOfColorsB[i] = t_color;
        }

        count--;
    }


    //Checks if either players can gobble using their reserves if there are 3 gobblets of the same colour in a row
    if (CheckReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::Black))
    {
        if (!m_player.canUseReserves())
        {
            m_player.setReserveUse(true);
        }
    }


    if (CheckReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::White))
    {
        if (!m_NPCPlayer.canUseReserves())
        {
            m_NPCPlayer.setReserveUse(true);
        }
    }


    //m_player.SetReserveUse(CheckReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::Black));
    //m_NPCPlayer.SetReserveUse(CheckReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::White));


    if (std::all_of(
        std::begin(diagonalOfColorsA),
        std::end(diagonalOfColorsA),
        [t_color](sf::Color i)
        {
            return i == t_color;
        }
    ))
    {
        std::cout << "4 in a diagonal row" << std::endl;
        return true;
    }


    if (std::all_of(
        std::begin(diagonalOfColorsB),
        std::end(diagonalOfColorsB),
        [t_color](sf::Color i)
        {
            return i == t_color;
        }
    ))
    {
        std::cout << "4 in a diagonal row" << std::endl;
        return true;
    }*/


    return false;
}

bool Board::CheckReserveUsage(sf::Color a[], sf::Color b[], sf::Color t_color)
{
    int count = 0;
    //Checks if the current player can gobble up a gobblet from their reserves
    for (size_t i = 0; i < 4; i++)
    {
        if (a[i] == t_color || b[i] == t_color)
        {
            count++;
            if (count == 3)
            {
                return true;
            }
        }
    }

    return false;
}

void Board::CheckTieCondition()
{
    //for (auto& stack : m_gobbletStacks)
    //{
    //    if (stack.get()->getGridPosition().has_value())
    //    {
    //        continue;
    //    }
    //    else
    //    {
    //        std::cout << "The show goes on" << std::endl;
    //    }
    //}

    //if (std::all_of(
    //    std::begin(m_gobbletStacks),
    //    std::end(m_gobbletStacks),
    //    [](GobbletStack i)
    //    {return i.getGridPosition().has_value() == true; }
    //))
    //{
    //    std::cout << "TIE COUNTDOWN STARTS NOW" << std::endl;
    //}
    //else
    //{
    //    std::cout << "The show goes on" << std::endl;
    //}
}


void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_grid, states);

    for (const auto& gobblet : m_maxPlayer.getExternalGobblets())
    {
        target.draw(*gobblet, states);
    }

    for (const auto& gobblet : m_minPlayer.getExternalGobblets())
    {
        target.draw(*gobblet, states);
    }

    for (const auto& gobblet : m_gobblets)
    {
        target.draw(*gobblet, states);
    }
}
