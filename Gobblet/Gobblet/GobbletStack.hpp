#pragma once
#include <stack>

#include "Gobblet.hpp"

#include "Grid.h"

class GobbletStack final : public sf::Drawable
{
public:
    GobbletStack();

    void add(Gobblet& t_gobblet);
    Gobblet& top() const;
    Gobblet& pop();
    bool isEmpty() const;

    void setPosition(const sf::Vector2f& t_position);
    sf::Vector2f getPosition() const;

    void setGridPosition(const std::optional<sf::Vector2i>& t_gridPosition, Grid& t_grid);
    std::optional<sf::Vector2i> getGridPosition() const;

    bool isExternalStack() const;
    
    friend bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs);

    friend bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs);

    bool isSelected = false;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::stack<std::reference_wrapper<Gobblet>> m_stack;

    sf::Vector2f m_position;
    std::optional<sf::Vector2i> m_gridPosition;
};
