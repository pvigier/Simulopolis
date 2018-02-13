#include "gui/GuiVBoxLayout.h"

GuiVBoxLayout::GuiVBoxLayout()
{
    //ctor
}

GuiVBoxLayout::~GuiVBoxLayout()
{
    //dtor
}

sf::Vector2f GuiVBoxLayout::getSize() const
{
    sf::Vector2f size;
    for (GuiWidget* widget : mWidgets)
    {
        size.x = std::max(size.x, widget->getSize().x);
        size.y += widget->getSize().y + mSpacing;
    }
    size.y -= mSpacing;
    return size;
}

void GuiVBoxLayout::setAlignment(VAlignment alignment)
{
    if (alignment != mAlignment)
    {
        mAlignment = alignment;
        update();
    }
}

void GuiVBoxLayout::align()
{
    sf::Vector2f offset = mPosition;
    if (mAlignment == VAlignment::Top)
    {
        for (std::size_t i = 0; i < mWidgets.size(); ++i)
        {
            mWidgets[i]->setPosition(offset);
            offset.y += mWidgets[i]->getSize().y + mSpacing;
        }
    }
    else if (mAlignment == VAlignment::Center)
    {
        sf::Vector2f size = getSize();
        sf::Vector2f offset = mPosition;
    }
    else
    {
        sf::Vector2f size = getSize();
        sf::Vector2f offset = mPosition;
        offset.y += size.y;
        for (std::size_t i = mWidgets.size() - 1; i >= 0; --i)
        {
            offset.y -= mWidgets[i]->getSize().y;
            mWidgets[i]->setPosition(offset);
            offset.y -= mSpacing;
        }
    }
}
