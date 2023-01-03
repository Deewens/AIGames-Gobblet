﻿#pragma once

#include "GobbletStack.hpp"
#include "Grid.h"

class Board final : public sf::Drawable
{
public:
    explicit Board(sf::RenderWindow& t_window);

    void update(sf::Time t_deltaTime);
    void removeStackIfEmpty(const std::shared_ptr<GobbletStack>& stackToRemove);

    /**
     * \brief Choose the gobblet to be moved. The gobblet is selected from mouse position
     * \return flag used for controlling code flow. If true, nothing should be called after this method, and the calling method should immediately break or return. (e.g.: should not called placeGobblet after this method if it return true)
     */
    bool chooseGobblet();
    /**
     * \brief Place the choose gobblet somewhere in the grid according to mouse position
     * \return flag used for controlling code flow. If true, nothing should be called after this method, and the calling method should immediately break or return
     */
    bool placeGobblet();

    void processMouse(sf::Event t_event);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    enum class ActionState
    {
        ChooseGobblet,
        PlaceGobblet
    };

    sf::RenderWindow& m_window;
    sf::FloatRect m_mousePosition;

    Grid m_grid;

    std::vector<std::shared_ptr<GobbletStack>> m_gobbletStacks;
    std::weak_ptr<GobbletStack> m_activeStack;

    ActionState m_gobbletActionState = ActionState::ChooseGobblet;
};