#include "gui/Gui.h"

Gui::Gui(sf::Vector2f dimensions, int padding, bool horizontalAlignment, const GuiStyle& style,
    std::vector<std::pair<std::string, std::string>> entries) :
    mDimensions(dimensions), mPadding(padding), mHorizontalAlignment(horizontalAlignment),
    mStyle(style), mVisible(false)

{
    // Construct each gui entry
    unsigned int characterSize = mDimensions.y - mStyle.borderSize - mPadding;
    for (std::pair<std::string, std::string>& entry : entries)
        mEntries.push_back(GuiEntry(mStyle, entry.first, mDimensions, characterSize, entry.second));
}

sf::Vector2f Gui::getSize()
{
    return sf::Vector2f(mDimensions.x, mDimensions.y * mEntries.size());
}

int Gui::getEntry(const sf::Vector2f mousePosition)
{
    if (mEntries.empty() || !mVisible)
        return -1;

    for (std::size_t i = 0; i < mEntries.size(); ++i)
    {
        if (mEntries[i].hitButton(mousePosition))
            return i;
    }

    return -1;
}

void Gui::setEntryText(std::size_t iEntry, std::string text)
{
    if (iEntry < mEntries.size())
        mEntries[iEntry].setText(text);
}

void Gui::setDimensions(sf::Vector2f dimensions)
{
    unsigned int characterSize = mDimensions.y - mStyle.borderSize - mPadding;
    for (GuiEntry& entry : mEntries)
        entry.resize(mDimensions, characterSize);
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!mVisible)
        return;

    // Draw each entry of the menu
    for (const GuiEntry& entry : mEntries)
        target.draw(entry);
}

void Gui::show()
{
    mVisible = true;
    sf::Vector2f offset = getOrigin();

    // Draw each entry of the menu
    for(GuiEntry& entry : mEntries)
    {
        // Compute the position of the entry
        entry.setPosition(-offset + getPosition());

        if(mHorizontalAlignment)
            offset.x -= mDimensions.x;
        else
            offset.y -= mDimensions.y;
    }
}

void Gui::hide()
{
    mVisible = false;
}

void Gui::highlight(std::size_t iEntry)
{
    for(std::size_t i = 0; i < mEntries.size(); ++i)
    {
        if(i == iEntry)
            mEntries[i].setHighlight(true);
        else
            mEntries[i].setHighlight(false);
    }
}

std::string Gui::activate(std::size_t iEntry)
{
    if(iEntry >= mEntries.size())
        return "null";
    return mEntries[iEntry].getMessage();
}

std::string Gui::activate(sf::Vector2f mousePosition)
{
    return activate(getEntry(mousePosition));
}

bool Gui::isVisible() const
{
    return mVisible;
}

std::size_t Gui::getNbEntries() const
{
    return mEntries.size();
}
