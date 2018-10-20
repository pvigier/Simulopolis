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

#include "gui/GuiOverlapLayout.h"
#include "gui/GuiWidget.h"

GuiOverlapLayout::GuiOverlapLayout()
{
    //ctor
}

GuiOverlapLayout::GuiOverlapLayout(const PropertyList& properties) : GuiLayout(properties)
{

}

void GuiOverlapLayout::align()
{
    sf::Vector2f offset = mOwner->getInsidePosition() + sf::Vector2f(mMargins.left, mMargins.top);
    for (GuiWidget* widget : mOwner->getChildren())
        widget->setOutsidePosition(offset);
}

sf::Vector2f GuiOverlapLayout::computeSize() const
{
    sf::Vector2f size;
    for (const GuiWidget* widget : mOwner->getChildren())
    {
        size.x = std::max(size.x, widget->getOutsideSize().x);
        size.y = std::max(size.y, widget->getOutsideSize().y);
    }
    return size + sf::Vector2f(mMargins.left + mMargins.right, mMargins.top + mMargins.bottom);
}
