#include "Board.hpp"

#include "Gobblet.hpp"
#include "AI.hpp"

Board::Board(sf::RenderWindow& t_window) :
    m_window(t_window), m_mousePosition(0, 0, 1, 1),
    m_maxPlayer(sf::Color::Black, PlayerType::Max, false),
    m_minPlayer(sf::Color::White, PlayerType::Min, true),
    m_currentPlayer(PlayerType::Max)
{
    // Initialise the board, create external stacks around the grid
    std::list<std::shared_ptr<GobbletStack>> maxPlayerExternalStack = m_maxPlayer.initExternalStacks(m_grid);
    std::list<std::shared_ptr<GobbletStack>> minPlayerExternalStack = m_minPlayer.initExternalStacks(m_grid);
    m_gobbletStacks.splice(m_gobbletStacks.end(), maxPlayerExternalStack);
    m_gobbletStacks.splice(m_gobbletStacks.end(), minPlayerExternalStack);

    m_sameActionCount = 0;
}

void Board::update(sf::Time t_deltaTime)
{
    m_grid.update(m_window);

    const sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    m_mousePosition.left = mousePosition.x;
    m_mousePosition.top = mousePosition.y;

    if (m_currentPlayer == m_minPlayer.getType())
    {
        auto test = AI::findBestMove(*this, m_minPlayer);
    }
}

void Board::removeStackIfEmpty(const std::shared_ptr<GobbletStack>& stackToRemove)
{
    if (stackToRemove->isEmpty())
    {
        const auto newEndIt = m_gobbletStacks.erase(
            std::remove(m_gobbletStacks.begin(), m_gobbletStacks.end(), stackToRemove), m_gobbletStacks.end());
        if (newEndIt != m_gobbletStacks.end())
        {
            std::cerr << "Error while removing empty stack from vector" << std::endl;
        }
    }
}

void Board::activeGobbletStack(std::shared_ptr<GobbletStack> clickedStack)
{
    m_activeStack = clickedStack;
    m_activeStack.lock()->setClicked(true);
    m_activeStack.lock()->top().activateClickedState();
}

bool Board::chooseGobblet()
{
    if (m_gobbletActionState == ActionState::ChooseGobblet)
    {
        std::shared_ptr<GobbletStack> clickedStack;

        if (m_currentPlayer == PlayerType::Max)
        {
            if (m_maxPlayer.isAI())
                return false;

            clickedStack = findClickedGobbletStack(m_maxPlayer);
        }
        else
        {
            if (m_minPlayer.isAI())
                return false;

            clickedStack = findClickedGobbletStack(m_minPlayer);
        }

        if (clickedStack != nullptr)
        {
            if (!m_activeStack.expired())
            {
                m_activeStack.lock()->setClicked(false);
                m_activeStack.lock()->top().deactivateClickedState();
            }

            activeGobbletStack(clickedStack);

            if (m_activeStack.lock()->isExternalStack)
            {
                m_gobbletActionState = ActionState::PlaceBoardGobblet;
            }
            else
            {
                m_gobbletActionState = ActionState::PlaceGobblet;
            }

            return true;
        }
    }

    return false;
}

void Board::switchPlayer()
{
    if (m_currentPlayer == PlayerType::Max)
    {
        m_currentPlayer = PlayerType::Min;
    }
    else
    {
        m_currentPlayer = PlayerType::Max;
    }
}

bool Board::placeGobblet()
{
    if (m_gobbletActionState == ActionState::PlaceGobblet || m_gobbletActionState == ActionState::PlaceBoardGobblet)
    {
        // If mouse clicked on a tile of the grid, move the top gobblet from the active gobblet stack to the tile
        if (!m_activeStack.expired())
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
                            if (tile.gobbletStack.expired())
                            {
                                //moveGobblet(*m_activeStack, sf::Vector2i(static_cast<int>(y), static_cast<int>(x)));

                                moveGobblet(m_activeStack.lock(), &tile);
                            }
                            else // If a gobblet stack already exist on this tile, just add the active gobblet to it
                            {
                                if (m_gobbletActionState == ActionState::PlaceGobblet)
                                {
                                    if (m_maxPlayer.canUseReserves())
                                    {
                                        auto activeGobblet = m_activeStack.lock()->top();
                                        auto tileGobblet = tile.gobbletStack.lock()->top();
                                        if (activeGobblet.getSize() > tileGobblet.getSize())
                                        {
                                            moveGobblet(m_activeStack.lock(), &tile);

                                            m_maxPlayer.setReserveUse(false);
                                        }
                                        else
                                        //Guard that breaks out and returns to the loop if both gobblets are of the same size
                                        {
                                            break;
                                        }
                                    }
                                    else if (m_minPlayer.canUseReserves())
                                    {
                                        auto activeGobblet = m_activeStack.lock()->top();
                                        auto tileGobblet = tile.gobbletStack.lock()->top();
                                        if (activeGobblet.getSize() > tileGobblet.getSize())
                                        {
                                            moveGobblet(m_activeStack.lock(), &tile);

                                            m_minPlayer.setReserveUse(false);
                                        }
                                        else
                                        // Guard that breaks out and returns to the loop if both gobblets are of the same size
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
                                    auto activeGobblet = m_activeStack.lock()->top();
                                    auto tileGobblet = tile.gobbletStack.lock()->top();
                                    if (activeGobblet.getSize() > tileGobblet.getSize())
                                    {
                                        moveGobblet(m_activeStack.lock(), &tile);
                                    }
                                    else
                                    //Guard that breaks out and returns to the loop if both gobblets are of the same size
                                    {
                                        break;
                                    }
                                }
                            }

                            m_gobbletActionState = ActionState::ChooseGobblet;

                            switchPlayer();

                            if (CheckWinCondition(m_maxPlayer.getColor()))
                            {
                                std::cout << "Player wins" << std::endl;
                            }
                            if (CheckWinCondition(m_minPlayer.getColor()))
                            {
                                std::cout << "NPC Player wins" << std::endl;
                            }

                            return true;
                        }
                    }
                }
            }
            else if (m_gobbletActionState == ActionState::PlaceGobblet)
            {
                deactivateGobbletStack();
            }
        }
    }
    return false;
}

std::shared_ptr<GobbletStack> Board::findClickedGobbletStack(const Entity& t_player) const
{
    for (auto& stack : m_gobbletStacks)
    {
        if (stack->top().getShape().getGlobalBounds().intersects(m_mousePosition) && stack->top().getColor() ==
            t_player.getColor())
        {
            return stack;
        }
    }

    return nullptr;
}

void Board::deactivateGobbletStack()
{
    if (!m_activeStack.expired())
    {
        m_activeStack.lock()->setClicked(false);
        m_activeStack.lock()->top().deactivateClickedState();
        m_activeStack.reset();
        m_gobbletActionState = ActionState::ChooseGobblet;
    }
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
            if (!m_activeStack.expired())
            {
                deactivateGobbletStack();
            }
        }
    }
}


bool Board::CheckWinCondition(sf::Color t_color)
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
            if (!tile.gobbletStack.expired() && tile.gobbletStack.lock()->top().getShape().getFillColor() ==
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


            if (!tileB.gobbletStack.expired() && tileB.gobbletStack.lock()->top().getShape().getFillColor() ==
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
            if (!m_maxPlayer.canUseReserves())
            {
                m_maxPlayer.setReserveUse(true);
            }
        }


        if (CheckReserveUsage(rowOfColors, colOfColors, sf::Color::White))
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

        if (!tile.gobbletStack.expired() && tile.gobbletStack.lock()->top().getShape().getFillColor() == t_color)
        {
            diagonalOfColorsA[i] = t_color;
        }

        auto& tileB = gridArray[count][i];

        if (!tileB.gobbletStack.expired() && tileB.gobbletStack.lock()->top().getShape().getFillColor() ==
            t_color)
        {
            diagonalOfColorsB[i] = t_color;
        }

        count--;
    }


    //Checks if either players can gobble using their reserves if there are 3 gobblets of the same colour in a row
    if (CheckReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::Black))
    {
        if (!m_maxPlayer.canUseReserves())
        {
            m_maxPlayer.setReserveUse(true);
        }
    }


    if (CheckReserveUsage(diagonalOfColorsA, diagonalOfColorsB, sf::Color::White))
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

bool Board::moveGobblet(GobbletStack& t_gobbletStack, sf::Vector2i t_gridPosition)
{
    return false;
    /*if (t_gridPosition.x < 0 || t_gridPosition.x > Grid::Size || t_gridPosition.y < 0 || t_gridPosition.y > Grid::Size)
    {
        return false;
    }

    auto& gobbletToMove = t_gobbletStack.pop(); // Get gobblet reference to move and remove from stack
    auto& tile = m_grid.getGridArray()[t_gridPosition.y][t_gridPosition.x];

    if (tile.gobbletStack.expired()) // There is no stack on this tile
    {
        GobbletStack newStack;
        newStack.add(gobbletToMove);
        newStack.setGridPosition(t_gridPosition, m_grid);
        tile.gobbletStack = m_gobbletStacks.emplace_back(std::make_shared<GobbletStack>(newStack));

        removeStackIfEmpty(t_gobbletStack);
        return true;
    }
    else // A stack already exist on this tile
    {
        const auto stackOnNextTile = tile.gobbletStack.lock();
        if (gobbletToMove.getSize() > stackOnNextTile->top().getSize())
        {
            stackOnNextTile->add(gobbletToMove);
            removeStackIfEmpty(t_gobbletStack);
            return true;
        }
    }

    return false;*/
}

void Board::moveGobblet(const std::shared_ptr<GobbletStack>& t_gobbletStack, Tile* t_toTile)
{
    //auto& gobbletToMove = t_gobbletStack.pop(); // Get gobblet reference to move and remove from stack

    if (t_toTile->gobbletStack.expired())
    {
        deactivateGobbletStack();
        GobbletStack newStack;
        newStack.add(t_gobbletStack->pop());
        newStack.setGridPosition(m_grid, t_toTile->GetGridCoordinate());
        t_toTile->gobbletStack = m_gobbletStacks.emplace_back(std::make_shared<GobbletStack>(newStack));


        removeStackIfEmpty(t_gobbletStack);
    }
    else
    {
        if (t_gobbletStack->top().getSize() > t_toTile->gobbletStack.lock()->top().getSize())
        {
            deactivateGobbletStack();
            t_toTile->gobbletStack.lock()->add(t_gobbletStack->pop());
            t_toTile->gobbletStack.lock()->setGridPosition(m_grid, t_toTile->GetGridCoordinate());


            removeStackIfEmpty(t_gobbletStack);
        }
    }
}

void Board::moveGobblet(const GobbletStack& t_gobbletStack, const Tile& t_toTile)
{
    const auto foundStack = std::find_if(m_gobbletStacks.begin(), m_gobbletStacks.end(),
                                         [&t_gobbletStack](const auto& i)
                                         {
                                             return i.get() == &t_gobbletStack;
                                         });

    // Find the tile in the grid array
    Tile* foundTile = nullptr;
    for (int y = 0; y < m_grid.getGridArray().size(); y++)
    {
        for (int x = 0; x < m_grid.getGridArray()[y].size(); x++)
        {
            if (m_grid.getGridArray()[y][x] == t_toTile)
            {
                foundTile = &m_grid.getGridArray()[y][x];
            }
        }
    }

    if (foundStack != m_gobbletStacks.end() && foundTile != nullptr)
    {
        moveGobblet(*foundStack, foundTile);
    }
}

Grid& Board::getGrid()
{
    return m_grid;
}

const Grid& Board::getGrid() const
{
    return m_grid;
}

Entity Board::getOpponent(const Entity& t_player) const
{
    if (t_player == m_maxPlayer)
    {
        return m_minPlayer;
    }
    else
    {
        return m_maxPlayer;
    }
}

std::list<std::shared_ptr<GobbletStack>> Board::getGobbletStacks()
{
    return m_gobbletStacks;
}

const std::list<std::shared_ptr<GobbletStack>>& Board::getGobbletStacks() const
{
    return m_gobbletStacks;
}

void Board::moveGobblet(Gobblet& t_gobblet, Tile* t_tile)
{
    if (t_tile != nullptr) // Come from a tile
    {
        moveGobblet(t_gobblet.getStack().lock(), t_tile);
    }
    else // Come from external stack
    {
        
    }
}


void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_grid, states);

    for (const auto& stack : m_gobbletStacks)
    {
        target.draw(*stack, states);
    }
}
