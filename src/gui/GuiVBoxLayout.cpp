#include "gui/GuiVBoxLayout.h"

GuiVBoxLayout::GuiVBoxLayout()
{
    //ctor
}

GuiVBoxLayout::~GuiVBoxLayout()
{
    //dtor
}

void GuiVBoxLayout::align()
{
    sf::Vector2f offset = mPosition;
    sf::Vector2f size = computeSize();
    if (mVAlignment == VAlignment::Center)
        offset.y += mSize.y * 0.5f - size.y * 0.5f;
    else if (mVAlignment == VAlignment::Bottom)
        offset.y += mSize.y - size.y;
    for (GuiWidget* widget : mWidgets)
    {
        offset.x = mPosition.x;
        if (mHAlignment == HAlignment::Center)
            offset.x += mSize.x * 0.5f - widget->getSize().x * 0.5f;
        else if (mHAlignment == HAlignment::Right)
            offset.x += mSize.x - widget->getSize().x;
        widget->setPosition(offset);
        offset.y += widget->getSize().y + mSpacing;
    }
}

sf::Vector2f GuiVBoxLayout::computeSize() const
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
