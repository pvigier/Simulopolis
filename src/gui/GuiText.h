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

#include "SFML/Graphics.hpp"
#include "gui/GuiWidget.h"

class XmlDocument;
class PropertyList;

class GuiText : public GuiWidget
{
public:
    GuiText(const sf::String& text, unsigned int characterSize, const XmlDocument* style);
    GuiText(const PropertyList& properties);
    virtual ~GuiText();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setCharacterSize(unsigned int characterSize);
    const sf::Text& getText() const;
    const sf::String& getString() const;
    void setString(const sf::String& text);
    void setColor(sf::Color color);

protected:
    sf::Text mText;

    virtual void applyDesign() override;

private:
    sf::Vector2f computeSize() const;
};
