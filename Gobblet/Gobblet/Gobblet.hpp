#pragma once

#include <memory>
#include <stack>

#include <SFML/Graphics/Color.hpp>

class Gobblet
{
public:
    /**
     * \brief Create a new gobblet
     * \param t_color Color of this gobblet (either black or white)
     * \param t_size Gobblet size between 1 and 4
     */
    Gobblet(const sf::Color& t_color, int t_size);

    int getSize() const;

    /**
     * \brief Gobble up this gobblet with a bigger one
     * \param t_biggerGobblet Bigger gobblet
     * \return true if the passed gobblet is bigger than this one, false otherwise
     */
    bool gobbleUp(const std::shared_ptr<Gobblet>& t_biggerGobblet);

    std::shared_ptr<Gobblet> getParentGobblet();
    std::shared_ptr<Gobblet> getGobbledUpGobblet();

private:
    
    /**
    * \brief Check if the size of the gobblet is between 1 and 4.
    * \details If the size is more than 4, then the size will be set to 4. If the size lower than 1, then the size will be set to 1.
    */
    void verifySize();

    sf::Color m_color;
    int m_size;

    std::shared_ptr<Gobblet> m_parentGobblet;
    std::shared_ptr<Gobblet> m_childGobblet;
};
