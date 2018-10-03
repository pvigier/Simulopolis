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

#include <unordered_map>
#include <sstream>
#include <regex>
#include <SFML/Graphics.hpp>
#include "gui/GuiLayout.h"

class TextureManager;
class FontManager;
class StylesheetManager;
class XmlDocument;

class PropertyList
{
public:
    PropertyList();
    ~PropertyList();

    static void setTextureManager(TextureManager* textureManager);
    static void setFontManager(FontManager* fontManager);
    static void setStylesheetManager(StylesheetManager* stylesheetManager);

    void add(const std::string& name, const std::string& value);

    bool has(const std::string& name) const;

    std::string get(const std::string& name) const;

    template<typename T>
    T get(const std::string& name) const
    {
        std::istringstream stream(mProperties.at(name));
        T value;
        stream >> value;
        return value;
    }

    template<typename T>
    T get(const std::string& name, const T& defaultValue) const
    {
        if (has(name))
            return get<T>(name);
        else
            return defaultValue;
    }

    const std::unordered_map<std::string, std::string>& getProperties() const;

    // Percentages
    bool isPercentage(const std::string& name) const;
    float getPercentage(const std::string& name) const;
    bool isPercentageVector(const std::string& name) const;
    sf::Vector2f getPercentageVector(const std::string& name) const;

private:
    static TextureManager* sTextureManager;
    static FontManager* sFontManager;
    static StylesheetManager* sStylesheetManager;
    static std::regex mPercentageRegex;
    static std::regex mPercentageVectorRegex;

    std::unordered_map<std::string, std::string> mProperties;
};

// Basic types

template<>
std::string PropertyList::get(const std::string& name) const;

template<>
int PropertyList::get(const std::string& name) const;

template<>
float PropertyList::get(const std::string& name) const;

// SFML

template<>
sf::String PropertyList::get(const std::string& name) const;

template<>
sf::Vector2i PropertyList::get(const std::string& name) const;

template<>
sf::Vector2f PropertyList::get(const std::string& name) const;

template<>
sf::IntRect PropertyList::get(const std::string& name) const;

template<>
sf::Color PropertyList::get(const std::string& name) const;

template<>
const sf::Texture& PropertyList::get(const std::string& name) const;

template<>
const sf::Font& PropertyList::get(const std::string& name) const;

// My types

template<>
const XmlDocument* PropertyList::get(const std::string& name) const;

template<>
GuiLayout::HAlignment PropertyList::get(const std::string& name) const;

template<>
GuiLayout::VAlignment PropertyList::get(const std::string& name) const;

template<>
GuiLayout::Margins PropertyList::get(const std::string& name) const;
