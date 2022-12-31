#pragma once
#include <stack>

#include "Gobblet.hpp"

class GobbletStack final : public sf::Drawable
{
public:
    GobbletStack();

    void add(const Gobblet& t_gobblet);
    Gobblet& top();
    void remove();

    void setPosition(const sf::Vector2f& t_position);
    
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::stack<Gobblet> m_stack;

    sf::Vector2f m_position;
    std::optional<sf::Vector2i> m_gridPosition;
};
