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

class GuiWidget;
class PropertyList;

class GuiLayout
{
public:
    enum class HAlignment{Left, Center, Right};
    enum class VAlignment{Top, Center, Bottom};

    struct Margins
    {
        float left;
        float top;
        float right;
        float bottom;
    };

    GuiLayout(Margins margins = Margins{0.0f, 0.0f, 0.0f, 0.0f});
    GuiLayout(const PropertyList& properties);
    virtual ~GuiLayout();

    virtual void align() = 0;
    virtual sf::Vector2f computeSize() const = 0;

    void setOwner(GuiWidget* owner);
    void setMargins(Margins margins);

protected:
    GuiWidget* mOwner;
    Margins mMargins;
};
