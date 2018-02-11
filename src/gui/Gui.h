#pragma once

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "gui/GuiStyle.h"
#include "gui/GuiEntry.h"

class Gui : public sf::Transformable, public sf::Drawable
{
public:
    Gui(sf::Vector2f dimensions, int padding, bool horizontalAlignment, const GuiStyle& style,
        std::vector<std::pair<std::string, std::string>> entries);

    sf::Vector2f getSize();

    // Return the entry that the mouse is hovering over. Returns
    //-1 if the mouse if outside of the Gui
    int getEntry(const sf::Vector2f mousePosition);

    // Change the text of an entry.
    void setEntryText(std::size_t iEntry, std::string text);

    // Change the entry dimensions.
    void setDimensions(sf::Vector2f dimensions);

    // Draw the menu.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void show();

    void hide();

    // Highlights an entry of the menu.
    void highlight(std::size_t iEntry);

    // Return the message bound to the entry.
    std::string activate(std::size_t iEntry);
    std::string activate(sf::Vector2f mousePosition);

    bool isVisible() const;
    std::size_t getNbEntries() const;

private:
    sf::Vector2f mDimensions;
    int mPadding;
    // If true the menu entries will be horizontally, not vertically, adjacent
    bool mHorizontalAlignment;
    GuiStyle mStyle;
    std::vector<GuiEntry> mEntries;
    bool mVisible;
};

using GuiSystem = std::map<std::string, Gui>;
