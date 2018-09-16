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
 
#include "gui/GuiHBoxLayout.h"
#include "gui/GuiWidget.h"

void GuiHBoxLayout::align()
{
    sf::Vector2f offset = mOwner->getInsidePosition() + sf::Vector2f(mMargins.left, mMargins.top);
    sf::Vector2f size = computeSize();
    if (mHAlignment == HAlignment::Center)
        offset.x += mOwner->getInsideSize().x * 0.5f - size.x * 0.5f;
    else if (mHAlignment == HAlignment::Right)
        offset.x += mOwner->getInsideSize().x - size.x;
    for (GuiWidget* widget : mOwner->getChildren())
    {
        offset.y = mOwner->getInsidePosition().y + mMargins.top;
        if (mVAlignment == VAlignment::Center)
            offset.y += (mOwner->getInsideSize().y - mMargins.top - mMargins.bottom) * 0.5f - widget->getOutsideSize().y * 0.5f;
        else if (mVAlignment == VAlignment::Bottom)
            offset.y += mOwner->getInsideSize().y - mMargins.top - mMargins.bottom - widget->getOutsideSize().y;
        widget->setOutsidePosition(offset);
        offset.x += widget->getOutsideSize().x + mSpacing;
    }
}

sf::Vector2f GuiHBoxLayout::computeSize() const
{
    sf::Vector2f size;
    for (const GuiWidget* widget : mOwner->getChildren())
    {
        size.x += widget->getOutsideSize().x + mSpacing;
        size.y = std::max(size.y, widget->getOutsideSize().y);
    }
    size.x -= mSpacing;
    return size + sf::Vector2f(mMargins.left + mMargins.right, mMargins.top + mMargins.bottom);
}
