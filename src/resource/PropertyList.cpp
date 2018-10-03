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

#include "PropertyList.h"
#include "util/debug.h"
#include "resource/TextureManager.h"
#include "resource/FontManager.h"
#include "resource/StylesheetManager.h"

TextureManager* PropertyList::sTextureManager = nullptr;
FontManager* PropertyList::sFontManager = nullptr;
StylesheetManager* PropertyList::sStylesheetManager = nullptr;
std::regex PropertyList::mPercentageRegex("[0-9]+(.[0-9]*)?%");
std::regex PropertyList::mPercentageVectorRegex("[0-9]+(.[0-9]*)?% [0-9]+(.[0-9]*)?%");

PropertyList::PropertyList()
{
    //ctor
}

PropertyList::~PropertyList()
{
    //dtor
}

void PropertyList::setTextureManager(TextureManager* textureManager)
{
    sTextureManager = textureManager;
}

void PropertyList::setFontManager(FontManager* fontManager)
{
    sFontManager = fontManager;
}

void PropertyList::setStylesheetManager(StylesheetManager* stylesheetManager)
{
    sStylesheetManager = stylesheetManager;
}

void PropertyList::add(const std::string& name, const std::string& value)
{
    mProperties[name] = value;
}

bool PropertyList::has(const std::string& name) const
{
    return mProperties.find(name) != mProperties.end();
}

std::string PropertyList::get(const std::string& name) const
{
    DEBUG_IF(!has(name), name << " is not a key of this property list.\n");
    return mProperties.at(name);
}

template<>
std::string PropertyList::get<std::string>(const std::string& name) const
{
    return mProperties.at(name);
}

template<>
int PropertyList::get(const std::string& name) const
{
    return std::stoi(mProperties.at(name));
}

template<>
float PropertyList::get(const std::string& name) const
{
    return std::stof(mProperties.at(name));
}

template<>
sf::String PropertyList::get(const std::string& name) const
{
    return mProperties.at(name);
}

template<>
sf::Vector2i PropertyList::get(const std::string& name) const
{
    std::istringstream stream(mProperties.at(name));
    std::string x, y;
    stream >> x >> y;
    return sf::Vector2i(std::stoi(x), std::stoi(y));
}

template<>
sf::Vector2f PropertyList::get(const std::string& name) const
{
    std::istringstream stream(mProperties.at(name));
    std::string x, y;
    stream >> x >> y;
    return sf::Vector2f(std::stof(x), std::stof(y));
}

template<>
sf::IntRect PropertyList::get(const std::string& name) const
{
    std::istringstream stream(mProperties.at(name));
    std::string x, y, width, height;
    stream >> x >> y >> width >> height;
    return sf::IntRect(std::stoi(x), std::stoi(y), std::stoi(width), std::stoi(height));
}

template<>
sf::Color PropertyList::get(const std::string& name) const
{
    std::istringstream stream(mProperties.at(name));
    std::string r, g, b, a;
    stream >> r >> g >> b;
    if (!(stream >> a))
        a = "0xff";
    return sf::Color(std::stoi(r, nullptr, 16), std::stoi(g, nullptr, 16), std::stoi(b, nullptr, 16),
        std::stoi(a, nullptr, 16));
}

template<>
const sf::Texture& PropertyList::get(const std::string& name) const
{
    return sTextureManager->getTexture(mProperties.at(name));
}

template<>
const sf::Font& PropertyList::get(const std::string& name) const
{
    return sFontManager->getFont(mProperties.at(name));
}

template<>
const XmlDocument* PropertyList::get(const std::string& name) const
{
    return sStylesheetManager->getStylesheet(mProperties.at(name));
}

template<>
GuiLayout::HAlignment PropertyList::get(const std::string& name) const
{
    if (mProperties.at(name) == "left")
        return GuiLayout::HAlignment::Left;
    else if (mProperties.at(name) == "center")
        return GuiLayout::HAlignment::Center;
    else
        return GuiLayout::HAlignment::Right;
}

template<>
GuiLayout::VAlignment PropertyList::get(const std::string& name) const
{
    if (mProperties.at(name) == "top")
        return GuiLayout::VAlignment::Top;
    else if (mProperties.at(name) == "center")
        return GuiLayout::VAlignment::Center;
    else
        return GuiLayout::VAlignment::Bottom;
}

template<>
GuiLayout::Margins PropertyList::get(const std::string& name) const
{
    std::istringstream stream(mProperties.at(name));
    std::string left, top, right, bottom;
    stream >> left >> top >> right >> bottom;
    return GuiLayout::Margins{std::stof(left), std::stof(top), std::stof(right), std::stof(bottom)};
}

const std::unordered_map<std::string, std::string>& PropertyList::getProperties() const
{
    return mProperties;
}

bool PropertyList::isPercentage(const std::string& name) const
{
    return std::regex_match(mProperties.at(name), mPercentageRegex);
}

float PropertyList::getPercentage(const std::string& name) const
{
    const std::string& value = mProperties.at(name);
    return std::stof(value.substr(0, value.size() - 1)) / 100.0f;
}

bool PropertyList::isPercentageVector(const std::string& name) const
{
    return std::regex_match(mProperties.at(name), mPercentageVectorRegex);
}

sf::Vector2f PropertyList::getPercentageVector(const std::string& name) const
{
    const std::string& value = mProperties.at(name);
    std::size_t iPercentage = value.find('%');
    return sf::Vector2f(std::stof(value.substr(0, iPercentage)) / 100.0f,
        std::stof(value.substr(iPercentage + 1)) / 100.0f);
}
