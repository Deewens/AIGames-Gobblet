#pragma once

#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>


class Grid;

/**
 * \brief Representation of a gobblet
 */
class Gobblet final : public sf::Drawable
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
     * \param t_coordinates Coordinate of the gobblet on the grid. Pass nullptr to remove the gobblet from the grid
     */
    void setGridCoordinates(const Grid& t_grid, std::optional<sf::Vector2i> t_coordinates);

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& t_position);

    sf::CircleShape getShape();
    const sf::CircleShape& getShape() const;

    /**
     * \brief Add a style effect around the gobblet to know that it has been clicked
     */
    void activateClickedState();

    /**
     * \brief Deactivate the style effect
     */
    void deactivateClickedState();

    friend bool operator==(const Gobblet& t_lhs, const Gobblet& t_rhs);

    friend bool operator!=(const Gobblet& t_lhs, const Gobblet& t_rhs);

private:
    /**
    * \brief Check if the size of the gobblet is between 1 and 4.
    * \details If the size is more than 4, then the size will be set to 4. If the size lower than 1, then the size will be set to 1.
    */
    void verifySize();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::optional<sf::Vector2i> m_gridCoordinates;

    int m_size;
    float m_sizeFactor = 4;

    sf::Color m_color;
    sf::CircleShape m_shape;
};
