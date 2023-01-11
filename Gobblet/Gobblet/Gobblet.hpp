﻿#pragma once

#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>


class Tile;
class Grid;

class Gobblet final : public sf::Drawable, public std::enable_shared_from_this<Gobblet>
{
public:
    /**
     * \brief Create a new gobblet
     * \param t_color Color of this gobblet (either black or white)
     * \param t_size Gobblet size between 1 and 4
     * \param t_grid Grid this gobblet is on
     */
    Gobblet(const sf::Color& t_color, int t_size);

    int getSize() const;

    const sf::Color& getColor() const;

    /**
     * \brief Get the grid coordinate of the gobblet
     * \return The coordinate of the gobblet on the grid.
     */
    std::optional<sf::Vector2i> getGridCoordinates() const;

    /**
     * \brief Set the grid coordinate of the gobblet
     * \param t_grid
     * \param t_coordinates Coordinate of the gobblet on the grid. Pass nullptr to remove the gobblet from the grid
     */
    void setGridCoordinates(const Grid& t_grid, std::optional<sf::Vector2i> t_coordinates);

    const std::weak_ptr<Gobblet>& getParentGobblet() const;
    const std::weak_ptr<Gobblet>& getChildGobblet() const;

    std::weak_ptr<Gobblet>& getParentGobblet();
    std::weak_ptr<Gobblet>& getChildGobblet();
    
    const Tile* getTile() const;
    Tile* getTile();

    void setParentGobblet(std::shared_ptr<Gobblet> t_parent);
    void setChildGobblet(std::shared_ptr<Gobblet> t_child);
    void setTile(Tile* t_tile);

    /**
     * \brief Gobble up this gobblet with a bigger one
     * \param t_biggerGobblet Bigger gobblet
     * \return true if the passed gobblet is bigger than this one, false otherwise
     */
    bool gobbleUp(const Grid& t_grid, const std::shared_ptr<Gobblet>& t_biggerGobblet);

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& t_position);

    sf::CircleShape& getShape();

    void deactivateClickedState();
    void activateClickedState();

    friend bool operator==(const Gobblet& t_lhs, const Gobblet& t_rhs);

    friend bool operator!=(const Gobblet& t_lhs, const Gobblet& t_rhs);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    /**
    * \brief Check if the size of the gobblet is between 1 and 4.
    * \details If the size is more than 4, then the size will be set to 4. If the size lower than 1, then the size will be set to 1.
    */
    void verifySize();

    std::optional<sf::Vector2i> m_gridCoordinates;
    // If std::nullopt, then the gobblet is not on the grid (it is an external gobblet)
    Tile* m_tile; // If nullptr, then the gobblet is not on a tile (it is an external gobblet)

    std::weak_ptr<Gobblet> m_parentGobblet; // If null, top pointer
    std::weak_ptr<Gobblet> m_childGobblet; // If null, bottom pointer

    int m_size;
    const float m_sizeFactor = 4;

    sf::Color m_color;
    sf::CircleShape m_shape;
};
