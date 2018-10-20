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
    enum class Alignment{Left, Justified};

    GuiText(float width, const sf::String& string, unsigned int characterSize, Alignment alignment,
        const XmlDocument* style);
    GuiText(const PropertyList& properties);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void setUp() override;

    //void setCharacterSize(unsigned int characterSize);
    //const sf::String& getString() const;
    //void setString(const sf::String& string);

protected:
    using Line = std::vector<std::string>;
    using Paragraph = std::vector<Line>;

    sf::String mString;
    unsigned int mCharacterSize;
    Alignment mAlignment;
    std::vector<Paragraph> mParagraphs;
    std::vector<sf::Text> mTexts;

    virtual void applyDesign() override;
    virtual void applyStyle() override;

private:
    void buildParagraphs();
    void buildTexts();
    sf::Text createText(const std::string& string) const;
    unsigned int getLineHeight() const;
};
