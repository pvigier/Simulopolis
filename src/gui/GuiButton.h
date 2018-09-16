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

#include <string>
#include "gui/GuiWidget.h"

class XmlDocument;

class GuiButton : public GuiWidget
{
public:
    enum class State{NORMAL, HOVERED, PRESSED, FORCE_PRESSED, DISABLED};

    GuiButton(const XmlDocument* style);
    GuiButton(const PropertyList& properties);

    void setState(State state);

    bool hitButton(sf::Vector2f position) const;

protected:
    virtual bool onHover(sf::Vector2f position, bool processed) override;
    virtual bool onPress(sf::Vector2f position, bool processed) override;
    virtual bool onRelease(sf::Vector2f position, bool processed) override;

private:
    State mState;
};
