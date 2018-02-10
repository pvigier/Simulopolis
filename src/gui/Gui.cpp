#include "Gui.h"

Gui::Gui(sf::Vector2f dimensions, int padding, bool horizontalAlignment, const GuiStyle& style,
    std::vector<std::pair<std::string, std::string>> entries) :
    mDimensions(dimensions), mPadding(padding), mHorizontalAlignment(horizontalAlignment),
    mStyle(style), mVisible(false)

{
    // Construct the background shape
    sf::RectangleShape shape;
    shape.setSize(mDimensions);
    shape.setFillColor(mStyle.bodyColor);
    shape.setOutlineThickness(-mStyle.borderSize);
    shape.setOutlineColor(mStyle.borderColor);

    // Construct each gui entry
    unsigned int characterSize = mDimensions.y - mStyle.borderSize - mPadding;
    for (std::pair<std::string, std::string>& entry : entries)
    {
        // Construct the text
        sf::Text text;
        text.setString(entry.first);
        text.setFont(*style.font);
        text.setFillColor(mStyle.textColor);
        text.setCharacterSize(characterSize);

        mEntries.push_back(GuiEntry(shape, text, entry.second));
    }
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
        if (mEntries[i].getShape().getGlobalBounds().contains(mousePosition))
            return i;
    }

    return -1;
}

void Gui::setEntryText(std::size_t iEntry, std::string text)
{
    if (iEntry < mEntries.size())
        mEntries[iEntry].getText().setString(text);
}

void Gui::setDimensions(sf::Vector2f dimensions)
{
    mDimensions = dimensions;
    unsigned int characterSize = mDimensions.y - mStyle.borderSize - mPadding;
    for (GuiEntry& entry : mEntries)
    {
        entry.getShape().setSize(dimensions);
        entry.getText().setCharacterSize(characterSize);
    }
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!mVisible)
        return;

    // Draw each entry of the menu
    for (const GuiEntry& entry : mEntries)
    {
        // Draw the entry
        target.draw(entry.getShape());
        target.draw(entry.getText());
    }
}

void Gui::show()
{
    mVisible = true;
    sf::Vector2f offset;

    // Draw each entry of the menu
    for(GuiEntry& entry : mEntries)
    {
        // Set the origin of the entry
        sf::Vector2f origin = getOrigin();
        origin -= offset;
        entry.getShape().setOrigin(origin);
        entry.getText().setOrigin(origin);

        // Compute the position of the entry
        entry.getShape().setPosition(getPosition());
        entry.getText().setPosition(getPosition());

        if(mHorizontalAlignment)
            offset.x += mDimensions.x;
        else
            offset.y += mDimensions.y;
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
        {
            mEntries[i].getShape().setFillColor(mStyle.bodyHighlightColor);
            mEntries[i].getShape().setOutlineColor(mStyle.borderHighlightColor);
            mEntries[i].getText().setFillColor(mStyle.textHighlightColor);
        }
        else
        {
            mEntries[i].getShape().setFillColor(mStyle.bodyColor);
            mEntries[i].getShape().setOutlineColor(mStyle.borderColor);
            mEntries[i].getText().setFillColor(mStyle.textColor);
        }
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
