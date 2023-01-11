#pragma once

#include <stack>
#include <functional>

#include "Gobblet.hpp"

class GobbletStack final : public sf::Drawable
{
public:
    explicit GobbletStack(bool t_isExternalStack);

    void add(const Gobblet& t_gobblet);
    
    Gobblet& top();
    const Gobblet& top() const;
    Gobblet pop();
    
    bool isEmpty() const;

    void setPosition(sf::Vector2f t_position);
    void setGridPosition(const Grid& t_grid, const std::optional<sf::Vector2i>& t_gridPosition);

    sf::Vector2f getPosition();
    const sf::Vector2f& getPosition() const;

    std::vector<Gobblet> container();
    const std::vector<Gobblet>& container() const;
    
    bool isClicked() const;
    void setClicked(bool t_isClicked);
    
    bool isExternalStack = false;

    friend bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs);
    friend bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void setGobbletsPosition(sf::Vector2f t_position);
    void setGobbletsGridPosition(const Grid& t_grid, const std::optional<sf::Vector2i>& t_gridPosition);
    
    std::vector<Gobblet> m_stack;

    bool m_isClicked = false;

    sf::Vector2f m_position;
    std::optional<sf::Vector2i> m_gridPosition;
};