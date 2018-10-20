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

#include <regex>
#include <SFML/System/Clock.hpp>
#include "gui/GuiWidget.h"
#include "gui/GuiLayout.h"

class XmlDocument;
class GuiLabel;

class GuiInput : public GuiWidget
{
public:
    GuiInput(unsigned int characterSize, const XmlDocument* style);
    GuiInput(const PropertyList& properties);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void setUp() override;

    const sf::String& getString() const;
    bool setString(const sf::String& string);
    void setRegex(const std::string& pattern);

protected:
    virtual void onOutsidePositionChanged() override;
    virtual bool onPress(sf::Vector2f position, bool processed) override;
    virtual bool onKey(sf::Keyboard::Key key, bool processed) override;
    virtual bool onText(sf::Uint32 unicode, bool processed) override;

    virtual void applyStyle() override;

private:
    bool mFocus;
    unsigned int mCharacterSize;
    GuiLayout::Margins mMargins;
    const XmlDocument* mTextStyle;
    GuiLabel* mLabel;
    std::size_t mCursor;
    sf::RectangleShape mCursorShape;
    mutable sf::Int32 mElapsedTime;
    mutable sf::Clock mClock;
    std::regex mRegex;

    void resetClock() const;
    void setCursor(std::size_t cursor);
    std::size_t mousePositionToCursor(sf::Vector2f position);
};
