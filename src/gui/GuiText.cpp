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

#include "gui/GuiText.h"
#include <numeric>
#include "util/string.h"
#include "resource/XmlDocument.h"

GuiText::GuiText(float width, const sf::String& string, unsigned int characterSize, Alignment alignment,
    const XmlDocument* style) :
    GuiWidget(style), mString(string), mCharacterSize(characterSize), mAlignment(alignment)
{
    setFixedInsideWidth(width);
    setBackgroundColor(sf::Color::Green);
}

GuiText::GuiText(const PropertyList& properties) : GuiWidget(properties)
{
    mString = properties.get<sf::String>("string", "");
    mCharacterSize = properties.get<unsigned int>("characterSize", 0);
}

GuiText::~GuiText()
{
    //dtor
}

void GuiText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::draw(target, states);
    for (const sf::Text& text : mTexts)
        target.draw(text, states);
}

void GuiText::setUp()
{
    buildLines();
    buildTexts();
    applyStyle();
}

void GuiText::applyDesign()
{
    GuiWidget::applyDesign();
    sf::Vector2f position = mInsidePosition;
    unsigned int lineHeight = getLineHeight();
    if (mAlignment == Alignment::Left)
    {
        for (sf::Text& text : mTexts)
        {
            text.setPosition(sf::Vector2f(sf::Vector2i(position)));
            position.y += lineHeight;
        }
    }
    else if (mAlignment == Alignment::Justified)
    {
        std::size_t i = 0;
        for (int j = 0; j < static_cast<int>(mLines.size()) - 1; ++j)
        {
            const std::vector<std::string>& line = mLines[j];
            // Compute the space between word
            float width = std::accumulate(std::next(mTexts.begin(), i), std::next(mTexts.begin(), i + line.size()), 0.0f,
                [](const float& lhs, const sf::Text& rhs) { return lhs + rhs.getGlobalBounds().width; });
            float space = (mInsideSize.x - width) / (line.size() - 1);
            // Place the words
            for (std::size_t k = 0; k < line.size(); ++k)
            {
                mTexts[i].setPosition(sf::Vector2f(sf::Vector2i(position)));
                position.x += space + mTexts[i].getGlobalBounds().width;
                ++i;
            }
            position.x = mInsidePosition.x;
            position.y += lineHeight;
        }
        // Special case for the last line
        if (!mLines.empty())
            mTexts.back().setPosition(sf::Vector2f(sf::Vector2i(position)));
    }
}

void GuiText::applyStyle()
{
    GuiWidget::applyStyle();
    if (mStyle)
    {
        for (sf::Text& text : mTexts)
        {
            //line.setFont(mStyle->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"));
            text.setFillColor(mStyle->getFirstChildByName("text").getAttributes().get<sf::Color>("color"));
        }
    }
}

void GuiText::buildLines()
{
    mLines.clear();
    // Split string in words
    std::vector<std::string> words = split(mString.toAnsiString(), ' ');
    // Create lines
    if (!words.empty())
    {
        std::vector<std::string> line = {words.front()};
        std::string lineString = words.front();
        sf::Text lineText = createText(words.front());
        for (std::size_t i = 1; i < words.size(); ++i)
        {
            const std::string& word = words[i];
            lineString += ' ' + word;
            lineText.setString(lineString);
            if (lineText.getGlobalBounds().width > mInsideSize.x)
            {
                mLines.push_back(line);
                line = {word};
                lineText.setString(word);
                lineString = word;
            }
            else
                line.push_back(word);
        }
        if (!line.empty())
            mLines.push_back(line);
    }
    // Set height
    setFixedInsideHeight(mLines.size() * getLineHeight());
}

void GuiText::buildTexts()
{
    mTexts.clear();
    // Create texts
    if (mAlignment == Alignment::Justified)
    {
        for (int i = 0; i < static_cast<int>(mLines.size()) - 1; ++i)
        {
            for (const std::string& word : mLines[i])
                mTexts.emplace_back(createText(word));
        }
        // Special case for the last line
        if (!mLines.empty())
            mTexts.emplace_back(createText(join(mLines.back(), ' ')));
    }
    else
    {
        for (const std::vector<std::string>& line : mLines)
            mTexts.emplace_back(createText(join(line, ' ')));
    }
}

sf::Text GuiText::createText(const std::string& string) const
{
    return sf::Text(string, mStyle->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"), mCharacterSize);
}

unsigned int GuiText::getLineHeight() const
{
    return mCharacterSize * 5 / 4;
}
