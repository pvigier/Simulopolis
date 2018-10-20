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

#include "gui/GuiLayout.h"

class PropertyList;

class GuiBoxLayout : public GuiLayout
{
public:
    GuiBoxLayout(HAlignment hAlignment = HAlignment::Left, VAlignment vAlignment = VAlignment::Top,
        float spacing = 0.0f, Margins margins = Margins{0.0f, 0.0f, 0.0f, 0.0f});
    GuiBoxLayout(float spacing, Margins margins = Margins{0.0f, 0.0f, 0.0f, 0.0f});
    GuiBoxLayout(const PropertyList& properties);

    virtual void setSpacing(float spacing);
    void setHAlignment(HAlignment alignment);
    void setVAlignment(VAlignment alignment);

protected:
    float mSpacing;
    HAlignment mHAlignment;
    VAlignment mVAlignment;
};
