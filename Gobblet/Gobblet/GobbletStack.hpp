#pragma once

#include <stack>
#include <functional>

#include "Gobblet.hpp"

#include "Grid.h"

class GobbletStack final : public sf::Drawable
{
public:
    GobbletStack();
    explicit GobbletStack(bool t_isExternalStack);

    void add(Gobblet& t_gobblet);
    Gobblet& top();
    Gobblet& pop();
    bool isEmpty() const;

    std::stack<std::reference_wrapper<Gobblet>>& stack();
    bool isExternalStack = false;

    friend bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs);
    friend bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::stack<std::reference_wrapper<Gobblet>> m_stack;
};
