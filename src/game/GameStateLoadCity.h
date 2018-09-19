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
class GuiButton;

class GameStateLoadCity : public GameState
{
public:
    GameStateLoadCity();
    virtual ~GameStateLoadCity();

    virtual void enter() override;
    virtual void handleMessages() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void exit() override;

    std::string getSelectedCity() const;

private:
    std::unique_ptr<Gui> mGui;
    std::vector<GuiButton*> mButtons;
    std::size_t mISelected;

    void createGui();
};
