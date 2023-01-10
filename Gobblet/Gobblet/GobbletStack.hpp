#pragma once

#include <stack>
#include <functional>

#include "Gobblet.hpp"

class GobbletStack final : public sf::Drawable
{
public:
    GobbletStack();
    explicit GobbletStack(bool t_isExternalStack);

    void add(Gobblet& t_gobblet);
    Gobblet& top() const;
    Gobblet& pop();
    bool isEmpty() const;

    void setPosition(sf::Vector2f t_position) const;
    void setGridPosition(Grid& t_grid, sf::Vector2i t_gridPosition) const;

    std::list<std::reference_wrapper<Gobblet>>& stack();
    
    bool isExternalStack = false;
    bool isClicked = false;

    friend bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs);
    friend bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::list<std::reference_wrapper<Gobblet>> m_stack;
};
