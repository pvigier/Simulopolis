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
 
#include "gui/GuiBoxLayout.h"
#include "resource/PropertyList.h"

GuiBoxLayout::GuiBoxLayout(HAlignment hAlignment, VAlignment vAlignment, float spacing, Margins margins) :
    GuiLayout(margins), mSpacing(spacing), mHAlignment(hAlignment), mVAlignment(vAlignment)
{
    //ctor
}

GuiBoxLayout::GuiBoxLayout(float spacing, Margins margins) :
    GuiLayout(margins), mSpacing(spacing),
    mHAlignment(GuiLayout::HAlignment::Left), mVAlignment(GuiLayout::VAlignment::Top)
{

}

GuiBoxLayout::GuiBoxLayout(const PropertyList& properties) : GuiLayout(properties)
{
    mHAlignment = properties.get("hAlignment", HAlignment::Left);
    mVAlignment = properties.get("vAlignment", VAlignment::Top);
    mSpacing = properties.get("spacing", 0.0f);
}

GuiBoxLayout::~GuiBoxLayout()
{
    //dtor
}

void GuiBoxLayout::setSpacing(float spacing)
{
    mSpacing = spacing;
}

void GuiBoxLayout::setHAlignment(HAlignment alignment)
{
    mHAlignment = alignment;
}

void GuiBoxLayout::setVAlignment(VAlignment alignment)
{
    mVAlignment = alignment;
}
