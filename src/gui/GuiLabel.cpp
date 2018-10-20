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

#include "gui/GuiLabel.h"
#include "resource/XmlDocument.h"

GuiLabel::GuiLabel(const sf::String& string, unsigned int characterSize, const XmlDocument* style) :
    GuiWidget(style),
    mText(string, mStyle->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"), characterSize)
{

}

GuiLabel::GuiLabel(const PropertyList& properties) : GuiWidget(properties)
{
    mText.setString(properties.get<sf::String>("string", ""));
    mText.setCharacterSize(properties.get<unsigned int>("characterSize", 0));
}

void GuiLabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::draw(target, states);
    target.draw(mText, states);
}

void GuiLabel::setUp()
{
    GuiWidget::setUp();
    setFixedInsideSize(computeSize());
}

void GuiLabel::setCharacterSize(unsigned int characterSize)
{
    mText.setCharacterSize(characterSize);
    setFixedInsideSize(computeSize());
}

const sf::Text& GuiLabel::getText() const
{
    return mText;
}

const sf::String& GuiLabel::getString() const
{
    return mText.getString();
}

void GuiLabel::setString(const sf::String& string)
{
    if (string != mText.getString())
    {
        mText.setString(string);
        setFixedInsideSize(computeSize());
    }
}

void GuiLabel::setColor(sf::Color color)
{
    mText.setFillColor(color);
}

void GuiLabel::applyDesign()
{
    mText.setPosition(sf::Vector2f(sf::Vector2i(mInsidePosition)));
}

void GuiLabel::applyStyle()
{
    GuiWidget::applyStyle();
    if (mStyle)
    {
        mText.setFont(mStyle->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"));
        mText.setFillColor(mStyle->getFirstChildByName("text").getAttributes().get<sf::Color>("color"));
    }
}

sf::Vector2f GuiLabel::computeSize() const
{
    return sf::Vector2f(mText.getGlobalBounds().width, mText.getCharacterSize() * 5 / 4);
}
