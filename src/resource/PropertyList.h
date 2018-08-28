#pragma once

#include <unordered_map>
#include <sstream>
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

private:
    static TextureManager* sTextureManager;
    static FontManager* sFontManager;
    static StylesheetManager* sStylesheetManager;

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
