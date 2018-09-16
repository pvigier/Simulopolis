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

#include "gui/GuiWidget.h"

class XmlDocument;

class GuiWindow : public GuiWidget
{
public:
    GuiWindow(const std::string& title, const XmlDocument* style);
    GuiWindow(const PropertyList& properties);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setTitle(const std::string& title);

    virtual bool hasGuiEvents() const override;

protected:
    virtual void onOutsidePositionChanged() override;
    virtual void onContentSizeChanged(sf::Vector2f contentSize) override;
    virtual bool onHover(sf::Vector2f position, bool processed) override;
    virtual bool onPress(sf::Vector2f position, bool processed) override;
    virtual bool onRelease(sf::Vector2f position, bool processed) override;

    virtual void applyStyle();

private:
    // Handles appearance of the window
    sf::RectangleShape mBar;
    sf::Text mTitle;
    sf::CircleShape mCloseButton;
    // Events
    bool mOnMove;
    sf::Vector2f mAnchor;
};
