/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"

class Gui;

class GameStateSettings : public GameState
{
public:
    GameStateSettings();
    virtual ~GameStateSettings();

    virtual void enter() override;
    virtual void handleMessages() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void exit() override;

    void setCityTexture(const sf::Texture& texture);

private:
    std::unique_ptr<Gui> mGui;
    // tmp
    bool mFullscreen;
    bool mMute;

    void createGui();
};
