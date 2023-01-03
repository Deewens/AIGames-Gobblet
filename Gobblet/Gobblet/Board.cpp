#include "Board.hpp"

#include "Gobblet.hpp"

Board::Board(sf::RenderWindow& t_window) : m_window(t_window), m_mousePosition(0, 0, 1, 1), m_activeStack(),
m_player(sf::Color::White,false),
m_NPCPlayer(sf::Color::Black,false)
{
    // Initialise the board, create external stacks around the grid
    const sf::Vector2f gridPosition = m_grid.getPosition();
    const sf::Vector2f gridSize = m_grid.getSize();

    float yPosition = gridPosition.y;
    for (int i = 0; i < 3; i++)
    {
        GobbletStack blackStack;
        GobbletStack whiteStack;

        yPosition = yPosition + 50;

        for (int j = 1; j <= 4; j++)
        {
            blackStack.add(Gobblet(Gobblet::Color::Black, j, m_grid));
            blackStack.setPosition({gridPosition.x - 50, yPosition}); // 50px - left side of the grid

            whiteStack.add(Gobblet(Gobblet::Color::White, j, m_grid));
            whiteStack.setPosition({gridPosition.x + (gridSize.x + 50), yPosition}); // 50px - right side of the grid
        }

        m_gobbletStacks.push_back(std::make_shared<GobbletStack>(blackStack));
        m_gobbletStacks.push_back(std::make_shared<GobbletStack>(whiteStack));
    }

    m_turnOrder = true;
}

void Board::update(sf::Time t_deltaTime)
{
    m_grid.update(m_window);

    const sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    m_mousePosition.left = mousePosition.x;
    m_mousePosition.top = mousePosition.y;
}

void Board::removeStackIfEmpty(const std::shared_ptr<GobbletStack>& stackToRemove)
{
    if (stackToRemove->isEmpty())
    {
        const auto newEndIt = m_gobbletStacks.erase(
            std::remove(m_gobbletStacks.begin(), m_gobbletStacks.end(),
                        stackToRemove), m_gobbletStacks.end());
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
                if (stack->top().getShape().getGlobalBounds().intersects(m_mousePosition) && stack->top().getShape().getFillColor() == m_player.GetColor())
                {
                    isGobbletClicked = true;

                    // Remove the "active effect" on the top gobblet from the currently active stack
                    if (!m_activeStack.expired()) // .expired weak_ptr equivalent for m_activeStack == nullptr
                    {
                        // .lock() convert the weak_ptr into a shared_ptr to be used as such
                        m_activeStack.lock()->top().deactivateClickedState();
                    }

                    // Set the new active stack and activate the "active effect" on its top

                    if (stack->top().getGridCoordinates().has_value())
                    {
                        m_activeStack = stack;
                        m_activeStack.lock()->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceBoardGobblet;
                    }
                    else
                    {
                        m_activeStack = stack;
                        m_activeStack.lock()->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceGobblet;
                    }
                    return true;
                }
            }

            if (!isGobbletClicked && !m_activeStack.expired())
            {
                m_activeStack.lock()->top().deactivateClickedState();
                m_activeStack.reset();

                m_gobbletActionState = ActionState::ChooseGobblet;
            }


        }

        //Player 2's turn
        else
        {
            for (auto& stack : m_gobbletStacks)
            {

                if (stack->top().getShape().getGlobalBounds().intersects(m_mousePosition) && stack->top().getShape().getFillColor() == m_NPCPlayer.GetColor())
                {
                    isGobbletClicked = true;

                    // Remove the "active effect" on the top gobblet from the currently active stack
                    if (!m_activeStack.expired()) // .expired weak_ptr equivalent for m_activeStack == nullptr
                    {
                        // .lock() convert the weak_ptr into a shared_ptr to be used as such
                        m_activeStack.lock()->top().deactivateClickedState();
                    }

                    // Set the new active stack and activate the "active effect" on its top

                    //If the gobblet picked is on the board, they are unable to deselect it
                    if (stack->top().getGridCoordinates().has_value())
                    {
                        m_activeStack = stack;
                        m_activeStack.lock()->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceBoardGobblet;
                    }
                    else
                    {
                        m_activeStack = stack;
                        m_activeStack.lock()->top().activateClickedState();
                        m_gobbletActionState = ActionState::PlaceGobblet;
                    }

                    return true;
                }
            }

            if (!isGobbletClicked && !m_activeStack.expired())
            {
                m_activeStack.lock()->top().deactivateClickedState();
                m_activeStack.reset();

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
                                m_activeStack.lock()->top().deactivateClickedState();

                                GobbletStack newStack;
                                newStack.add(m_activeStack.lock()->top());
                                m_activeStack.lock()->pop();
                                newStack.setGridPosition(sf::Vector2i(static_cast<int>(y), static_cast<int>(x)),
                                                         m_grid);

                                removeStackIfEmpty(m_activeStack.lock());
                                // Set it to nullptr, because it will be invalidated by push_back anyways and points to an invalid address
                                m_activeStack.reset();

                                // activeStack pointer is invalidated by emplace_back, because push_back reallocate memory, and activeStack points to a memory address stored in the gobbletStacks vector.
                                // So, it is important to call emplace_back after we did stuff with m_activeStack, because it is invalidated afterwards.
                                tile.gobbletStack = m_gobbletStacks.emplace_back(
                                    std::make_shared<GobbletStack>(newStack));
                            }
                            // If a gobblet stack already exist on this tile, just add the active gobblet to it
                            else
                            {
                                auto activeGobblet = m_activeStack.lock()->top();
                                auto tileGobblet = tile.gobbletStack.lock()->top();
                                if (activeGobblet.getSize() > tileGobblet.getSize())
                                {
                                    m_activeStack.lock()->top().deactivateClickedState();

                                    tile.gobbletStack.lock()->add(m_activeStack.lock()->top());
                                    m_activeStack.lock()->pop();

                                    removeStackIfEmpty(m_activeStack.lock());
                                    m_activeStack.reset();
                                }
                                else//Guard that breaks out and returns to the loop if both gobblets are of the same size
                                {
                                    break;
                                }
                            }

                            m_gobbletActionState = ActionState::ChooseGobblet;

                            m_turnOrder = !m_turnOrder;
                            return true;
                        }
                    }
                }
            }
            else if (m_gobbletActionState == ActionState::PlaceGobblet)
            {
                m_activeStack.lock()->top().deactivateClickedState();
                m_activeStack.reset();

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


void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_grid, states);

    for (const auto& stack : m_gobbletStacks)
    {
        target.draw(*stack.get(), states);
    }
}
