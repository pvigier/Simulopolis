#include "gui/Gui.h"

Gui::Gui(sf::Vector2f dimensions, int padding, bool horizontalAlignment, const GuiStyle& style,
    std::vector<std::pair<std::string, std::string>> entries) :
    mDimensions(dimensions), mPadding(padding), mHorizontalAlignment(horizontalAlignment),
    mStyle(style), mVisible(false)

{
    // Construct each gui button
    unsigned int characterSize = mDimensions.y - mStyle.borderSize - mPadding;
    for (std::pair<std::string, std::string>& button : entries)
        mButtons.push_back(GuiButton(mStyle, button.first, mDimensions, characterSize, button.second));
}

sf::Vector2f Gui::getSize()
{
    return sf::Vector2f(mDimensions.x, mDimensions.y * mButtons.size());
}

int Gui::getEntry(const sf::Vector2f mousePosition)
{
    if (mButtons.empty() || !mVisible)
        return -1;

    for (std::size_t i = 0; i < mButtons.size(); ++i)
    {
        if (mButtons[i].hitButton(mousePosition))
            return i;
    }

    return -1;
}

void Gui::setEntryText(std::size_t iEntry, std::string text)
{
    if (iEntry < mButtons.size())
        mButtons[iEntry].setText(text);
}

void Gui::setDimensions(sf::Vector2f dimensions)
{
    unsigned int characterSize = mDimensions.y - mStyle.borderSize - mPadding;
    for (GuiButton& button : mButtons)
        button.resize(mDimensions, characterSize);
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!mVisible)
        return;

    // Draw each button of the menu
    for (const GuiButton& button : mButtons)
        target.draw(button);
}

void Gui::show()
{
    mVisible = true;
    sf::Vector2f offset = getOrigin();

    // Draw each button of the menu
    for(GuiButton& button : mButtons)
    {
        // Compute the position of the button
        button.setPosition(-offset + getPosition());

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
    for(std::size_t i = 0; i < mButtons.size(); ++i)
    {
        if(i == iEntry)
            mButtons[i].setHighlight(true);
        else
            mButtons[i].setHighlight(false);
    }
}

std::string Gui::activate(std::size_t iEntry)
{
    if(iEntry >= mButtons.size())
        return "null";
    return mButtons[iEntry].getMessage();
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
    return mButtons.size();
}
