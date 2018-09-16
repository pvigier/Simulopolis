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
 
#include "gui/GuiLayout.h"
#include "resource/PropertyList.h"

GuiLayout::GuiLayout(Margins margins) : mOwner(nullptr), mMargins(margins)
{
    //ctor
}

GuiLayout::GuiLayout(const PropertyList& properties) : mOwner(nullptr)
{
    mMargins = properties.get<Margins>("margins", Margins{0.0f, 0.0f, 0.0f, 0.0f});
}

GuiLayout::~GuiLayout()
{
    //dtor
}

void GuiLayout::setOwner(GuiWidget* owner)
{
    mOwner = owner;
}

void GuiLayout::setMargins(Margins margins)
{
    mMargins = margins;
}
