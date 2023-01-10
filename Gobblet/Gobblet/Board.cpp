#include "Board.hpp"

#include "Gobblet.hpp"
#include "AI.hpp"

Board::Board(sf::RenderWindow& t_window) :
    m_window(t_window), m_mousePosition(0, 0, 1, 1),
    m_maxPlayer(sf::Color::Black, PlayerAIType::Max),
    m_minPlayer(sf::Color::White, PlayerAIType::Min)
{
    // Initialise the board, create external stacks around the grid
    m_maxPlayer.initExternalStacks(m_grid);
    m_maxPlayer.initExternalStacks(m_grid);

    m_turnOrder = true;
    m_sameActionCount = 0;
}

void Board::update(sf::Time t_deltaTime)
{
    m_grid.update(m_window);

    const sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    m_mousePosition.left = mousePosition.x;
    m_mousePosition.top = mousePosition.y;
}

void Board::removeStackIfEmpty(const GobbletStack& stackToRemove)
{
    if (stackToRemove.isEmpty())
    {
        const auto newEndIt = m_gobbletStacks.erase(
            std::remove(m_gobbletStacks.begin(), m_gobbletStacks.end(), stackToRemove));
        if (newEndIt != m_gobbletStacks.end())
        {
            std::cerr << "Error while removing empty stack from vector" << std::endl;
        }
    }
}

bool Board::chooseGobblet()
{
    if (m_gobbletActionState == ActionState::ChooseGobblet)
    {
        bool isGobbletClicked = false;

        //Player 1's turn
        if (m_turnOrder)
        {
            // Determine if mouse clicked on a GobbletStack
            for (auto& stack : m_gobbletStacks)
            {
                if (stack.top().getShape().getGlobalBounds().intersects(m_mousePosition) && stack.top().getShape().
                    getFillColor() == m_maxPlayer.getColor())
                {
                    isGobbletClicked = true;

                    // Remove the "active effect" on the top gobblet from the currently active stack
                    if (m_activeStack != nullptr) // .expired weak_ptr equivalent for m_activeStack == nullptr
                    {
                        // .lock() convert the weak_ptr into a shared_ptr to be used as such
                        m_activeStack->top().deactivateClickedState();
                    }

                    // Set the new active stack and activate the "active effect" on its top

                    if (stack.top().getGridCoordinates().has_value())
                    {
                        m_activeStack = &stack;
                        m_activeStack->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceBoardGobblet;
                    }
                    else
                    {
                        m_activeStack = &stack;
                        m_activeStack->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceGobblet;
                    }
                    return true;
                }
            }

            if (!isGobbletClicked && m_activeStack != nullptr)
            {
                m_activeStack->top().deactivateClickedState();
                m_activeStack = nullptr;

                m_gobbletActionState = ActionState::ChooseGobblet;
            }
        }

        //Player 2's turn
        else
        {
            for (auto& stack : m_gobbletStacks)
            {
                if (stack.top().getShape().getGlobalBounds().intersects(m_mousePosition) && stack.top().getShape().
                    getFillColor() == m_minPlayer.getColor())
                {
                    isGobbletClicked = true;

                    // Remove the "active effect" on the top gobblet from the currently active stack
                    if (m_activeStack != nullptr) // .expired weak_ptr equivalent for m_activeStack == nullptr
                    {
                        // .lock() convert the weak_ptr into a shared_ptr to be used as such
                        m_activeStack->top().deactivateClickedState();
                    }

                    // Set the new active stack and activate the "active effect" on its top

                    //If the gobblet picked is on the board, they are unable to deselect it
                    if (stack.top().getGridCoordinates().has_value())
                    {
                        m_activeStack = &stack;
                        m_activeStack->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceBoardGobblet;
                    }
                    else
                    {
                        m_activeStack = &stack;
                        m_activeStack->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceGobblet;
                    }

                    return true;
                }
            }

            if (!isGobbletClicked && m_activeStack != nullptr)
            {
                m_activeStack->top().deactivateClickedState();
                m_activeStack = nullptr;

                m_gobbletActionState = ActionState::ChooseGobblet;
            }
        }
    }
    return false;
}

bool Board::placeGobblet()
{
    if (m_gobbletActionState == ActionState::PlaceGobblet || m_gobbletActionState == ActionState::PlaceBoardGobblet)
    {
        // If mouse clicked on a tile of the grid, move the top gobblet from the active gobblet stack to the tile
        if (m_activeStack != nullptr)
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
                                m_activeStack->top().deactivateClickedState();

                                moveGobblet(*m_activeStack,
                                            sf::Vector2i(static_cast<int>(y), static_cast<int>(x)));

                                // Set it to nullptr, because it will be invalidated by push_back anyways and points to an invalid address
                                m_activeStack = nullptr;
                            }
                            // If a gobblet stack already exist on this tile, just add the active gobblet to it
                            else
                            {
                                if (m_gobbletActionState == ActionState::PlaceGobblet)
                                {
                                    if (m_maxPlayer.canUseReserves())
                                    {
                                        auto activeGobblet = m_activeStack->top();
                                        auto tileGobblet = tile.gobbletStack.lock()->top();
                                        if (activeGobblet.getSize() > tileGobblet.getSize())
                                        {
                                            m_activeStack->top().deactivateClickedState();

                                            moveGobblet(*m_activeStack,
                                                        sf::Vector2i(static_cast<int>(y), static_cast<int>(x)));

                                            // Set it to nullptr, because it will be invalidated by push_back anyways and points to an invalid address
                                            m_activeStack = nullptr;

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
                                        auto activeGobblet = m_activeStack->top();
                                        auto tileGobblet = tile.gobbletStack.lock()->top();
                                        if (activeGobblet.getSize() > tileGobblet.getSize())
                                        {
                                            m_activeStack->top().deactivateClickedState();

                                            moveGobblet(*m_activeStack,
                                                        sf::Vector2i(static_cast<int>(y), static_cast<int>(x)));

                                            // Set it to nullptr, because it will be invalidated by push_back anyways and points to an invalid address
                                            m_activeStack = nullptr;

                                            m_minPlayer.setReserveUse(false);
                                        }
                                        else
                                        //Guard that breaks out and returns to the loop if both gobblets are of the same size
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
                                    auto activeGobblet = m_activeStack->top();
                                    auto tileGobblet = tile.gobbletStack.lock()->top();
                                    if (activeGobblet.getSize() > tileGobblet.getSize())
                                    {
                                        m_activeStack->top().deactivateClickedState();

                                        moveGobblet(*m_activeStack,
                                                    sf::Vector2i(static_cast<int>(y), static_cast<int>(x)));

                                        // Set it to nullptr, because it will be invalidated by push_back anyways and points to an invalid address
                                        m_activeStack = nullptr;
                                    }
                                    else
                                    //Guard that breaks out and returns to the loop if both gobblets are of the same size
                                    {
                                        break;
                                    }
                                }
                            }

                            m_gobbletActionState = ActionState::ChooseGobblet;

                            m_turnOrder = !m_turnOrder;

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

Grid& Board::getGrid()
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


void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_grid, states);

    for (const auto& stack : m_gobbletStacks)
    {
        target.draw(stack, states);
    }
}
