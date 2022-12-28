#pragma once

#include <memory>
#include <optional>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>


class Grid;

class Gobblet final : public sf::Drawable, public std::enable_shared_from_this<Gobblet>, sf::NonCopyable
{
public:
    enum class Color { Black, White };

    /**
     * \brief Create a new gobblet
     * \param t_color Color of this gobblet (either black or white)
     * \param t_size Gobblet size between 1 and 4
     * \param t_grid Grid this gobblet is on
     */
    Gobblet(const Color& t_color, int t_size, Grid& t_grid);

    int getSize() const;

    /**
     * \brief Get the grid coordinate of the gobblet
     * \return The coordinate of the gobblet on the grid. If the gobblet is not on the grid, the returned value is nullptr
     */
    std::optional<sf::Vector2i> getGridCoordinates() const;

    /**
     * \brief Set the grid coordinate of the gobblet
     * \param t_coordinates Coordinate of the gobblet on the grid. Pass nullptr to remove the gobblet from the grid
     */
    void setGridCoordinates(std::optional<sf::Vector2i> t_coordinates);

    /**
     * \brief Gobble up this gobblet with a bigger one
     * \remark Should probably be private...
     * \param t_biggerGobblet Bigger gobblet
     * \return true if the passed gobblet is bigger than this one, false otherwise
     */
    bool gobbleUp(const std::shared_ptr<Gobblet>& t_biggerGobblet);

    std::shared_ptr<Gobblet> getParentGobblet();
    std::shared_ptr<Gobblet> getChildGobblet();

    sf::Vector2f getPosition() const;

private:
    /**
    * \brief Check if the size of the gobblet is between 1 and 4.
    * \details If the size is more than 4, then the size will be set to 4. If the size lower than 1, then the size will be set to 1.
    */
    void verifySize();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Grid& m_grid;

    std::optional<sf::Vector2i> m_gridCoordinates;

    std::shared_ptr<Gobblet> m_parentGobblet;
    std::shared_ptr<Gobblet> m_childGobblet;

    int m_size;
    const float m_sizeFactor = 4;

    sf::Color m_color;
    sf::CircleShape m_shape;
};
