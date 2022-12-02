#pragma once

#include <memory>
#include <stack>
#include <SFML/Graphics/CircleShape.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Gobblet : sf::Drawable
{
public:
    enum class Color { Black, White };
    
    /**
     * \brief Create a new gobblet
     * \param t_color Color of this gobblet (either black or white)
     * \param t_size Gobblet size between 1 and 4
     */
    Gobblet(const Color& t_color, int t_size);

    int getSize() const;

    sf::Vector2i getGridCoordinates() const;
    void setGridCoordinates(const sf::Vector2i& coordinates);

    /**
     * \brief Gobble up this gobblet with a bigger one
     * \param t_biggerGobblet Bigger gobblet
     * \return true if the passed gobblet is bigger than this one, false otherwise
     */
    bool gobbleUp(Gobblet& t_biggerGobblet);

    std::shared_ptr<Gobblet> getParentGobblet();
    std::shared_ptr<Gobblet> getGobbledUpGobblet();

    sf::Vector2f getPosition() const;
private:
    /**
    * \brief Check if the size of the gobblet is between 1 and 4.
    * \details If the size is more than 4, then the size will be set to 4. If the size lower than 1, then the size will be set to 1.
    */
    void verifySize();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Color m_color;
    int m_size;

    std::unique_ptr<sf::Vector2i> m_gridCoordinates;

    Gobblet& m_parentGobblet;
    std::shared_ptr<Gobblet> m_childGobblet;

    sf::CircleShape m_shape;
};
