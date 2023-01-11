#include "Board.hpp"

#include "AI.h"
#include "Gobblet.hpp"
#include "Tile.h"
#include "GobbletStack.hpp"

Board::Board(sf::RenderWindow& t_window) :
    m_window(t_window), m_mousePosition(0, 0, 1, 1),
    m_maxPlayer(sf::Color::Black, Entity::Type::Max, false, m_grid),
    m_minPlayer(sf::Color::White, Entity::Type::Min, true, m_grid)
{
    m_currentPlayer = &m_maxPlayer;
    m_sameActionCount = 0;
}

void Board::update(sf::Time t_deltaTime)
{
    m_grid.update(m_window);

    const sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    m_mousePosition.left = mousePosition.x;
    m_mousePosition.top = mousePosition.y;

    if (m_currentPlayer->isAI())
    {
        AI::findBestMove(*this, *m_currentPlayer);
    }
}

bool Board::chooseGobblet()
{
    if (m_gobbletActionState == ActionState::ChooseGobblet)
    {
        bool isGobbletClicked = false;

        if (m_activeStack != nullptr && !m_activeStack->isEmpty())
        {
            m_activeStack->top().deactivateClickedState();
        }

        for (GobbletStack& stack : m_currentPlayer->getExternalStacks())
        {
            if (stack.isEmpty())
            {
                continue;
            }

            if (stack.top().getShape().getGlobalBounds().intersects(m_mousePosition))
            {
                m_activeStack = &stack;
                stack.top().activateClickedState();
                m_gobbletActionState = ActionState::PlaceGobblet;

                return true;
            }
        }

        for (int y = 0; y < m_grid.getGridArray().size(); y++)
        {
            for (int x = 0; x < m_grid.getGridArray()[y].size(); x++)
            {
                Tile& tile = m_grid.getGridArray()[y][x];

                if (tile.getStack().isEmpty())
                {
                    continue;
                }

                if (tile.getStack().top().getColor() == m_currentPlayer->getColor() && tile.getStack().top().getShape().getGlobalBounds().intersects(m_mousePosition))
                {
                    m_activeStack = &tile.getStack();
                    m_activeStack->top().activateClickedState();
                    m_gobbletActionState = ActionState::PlaceBoardGobblet;
                    return true;

                }
            }
        }

        if (!isGobbletClicked && m_activeStack != nullptr && !m_activeStack->isEmpty())
        {
            m_activeStack->top().deactivateClickedState();
            m_activeStack = nullptr;

            m_gobbletActionState = ActionState::ChooseGobblet;
        }
    }

    return false;
}

void Board::switchCurrentPlayer()
{
    if (m_currentPlayer == &m_maxPlayer)
    {
        m_currentPlayer = &m_minPlayer;
    }
    else
    {
        m_currentPlayer = &m_maxPlayer;
    }
}

bool Board::placeGobblet()
{
    if (m_gobbletActionState == ActionState::PlaceGobblet || m_gobbletActionState == ActionState::PlaceBoardGobblet)
    {
        // If mouse clicked on a tile of the grid, move the top gobblet from the active gobblet stack to the tile
        if (m_activeStack != nullptr && !m_activeStack->isEmpty())
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
                            // If there is not gobblet stack on this tile, create a new one
                            if (tile.getStack().isEmpty())
                            {
                                moveGobblet(*m_activeStack, tile.getStack());

                                m_activeStack = nullptr;
                            }
                            // If a gobblet stack already exist on this tile, just add the active gobblet to it
                            else
                            {
                                if (m_gobbletActionState == ActionState::PlaceGobblet)
                                {
                                    if (m_currentPlayer->canUseReserves())
                                    {
                                        if (m_activeStack->top().getSize() > tile.getStack().top().getSize())
                                        {
                                            moveGobblet(*m_activeStack, tile.getStack());
                                            
                                            m_activeStack = nullptr;

                                            m_currentPlayer->setReserveUse(false);
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    if (m_activeStack->top().getSize() > tile.getStack().top().getSize())
                                    {
                                        moveGobblet(*m_activeStack, tile.getStack());
                                        
                                        m_activeStack = nullptr;
                                        m_gobbletActionState = ActionState::ChooseGobblet;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }

                            m_gobbletActionState = ActionState::ChooseGobblet;
                            switchCurrentPlayer();

                            if (checkWinCondition(m_currentPlayer->getColor()))
                            {
                                std::cout << (m_currentPlayer->getType() == Entity::Type::Max
                                                  ? "Max wins"
                                                  : "Min wins")
                                    << std::endl;
                            }
                            return true;
                        }
                    }
                }
            }
            else if (m_gobbletActionState == ActionState::PlaceGobblet)
            {
                m_activeStack->top().deactivateClickedState();
                m_activeStack = nullptr;

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
    }
}


bool Board::checkWinCondition(sf::Color t_color)
{
    auto& gridArray = m_grid.getGridArray();

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
            if (!tile.getStack().isEmpty() && tile.getStack().top().getShape().getFillColor() ==
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


            if (!tileB.getStack().isEmpty() && tileB.getStack().top().getShape().getFillColor() ==
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

        if (checkReserveUsage(rowOfColors, colOfColors, sf::Color::Black))
        {
            if (!m_maxPlayer.canUseReserves())
            {
                m_maxPlayer.setReserveUse(true);
            }
        }


        if (checkReserveUsage(rowOfColors, colOfColors, sf::Color::White))
        {
            if (!m_minPlayer.canUseReserves())
            {
                m_minPlayer.setReserveUse(true);
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

        if (!tile.getStack().isEmpty() && tile.getStack().top().getShape().getFillColor() == t_color)
        {
            diagonalOfColorsA[i] = t_color;
        }

        auto& tileB = gridArray[count][i];

        if (!tileB.getStack().isEmpty() && tileB.getStack().top().getShape().getFillColor() ==
            t_color)
        {
            diagonalOfColorsB[i] = t_color;
        }

        count--;
    }


    //Checks if either players can gobble using their reserves if there are 3 gobblets of the same colour in a row
    if (checkReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::Black))
    {
        if (!m_maxPlayer.canUseReserves())
        {
            m_maxPlayer.setReserveUse(true);
        }
    }


    if (checkReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::White))
    {
        if (!m_minPlayer.canUseReserves())
        {
            m_minPlayer.setReserveUse(true);
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
    }


    return false;
}

bool Board::checkReserveUsage(sf::Color a[], sf::Color b[], sf::Color t_color)
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

void Board::checkTieCondition()
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

Grid& Board::getGrid()
{
    return m_grid;
}

const Grid& Board::getGrid() const
{
    return m_grid;
}

Entity& Board::getOpponent(const Entity& t_entity)
{
    if (t_entity.getType() == m_maxPlayer.getType())
    {
        return m_minPlayer;
    }
    else
    {
        return m_maxPlayer;
    }
}

void Board::moveGobblet(GobbletStack& t_fromStack, GobbletStack& t_toStack)
{
    t_toStack.add(t_fromStack.pop());
    t_toStack.top().setPosition(t_toStack.getPosition());
    t_toStack.top().deactivateClickedState();
}


void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_grid, states);


    for (const auto& stack : m_maxPlayer.getExternalStacks())
    {
        if (stack.isEmpty())
        {
            continue;
        }

        target.draw(stack, states);
    }

    for (const auto& stack : m_minPlayer.getExternalStacks())
    {
        if (stack.isEmpty())
        {
            continue;
        }

        target.draw(stack, states);
    }

    for (int y = 0; y < m_grid.getGridArray().size(); y++)
    {
        for (int x = 0; x < m_grid.getGridArray()[y].size(); x++)
        {
            const Tile& tile = m_grid.getGridArray()[y][x];
            if (tile.getStack().isEmpty())
            {
                continue;
            }

            target.draw(tile.getStack().top(), states);
        }
    }
}
