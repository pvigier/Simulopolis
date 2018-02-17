#include "gui/GuiHBoxLayout.h"

GuiHBoxLayout::GuiHBoxLayout()
{
    //ctor
}

GuiHBoxLayout::~GuiHBoxLayout()
{
    //dtor
}

void GuiHBoxLayout::align()
{
    sf::Vector2f offset = mPosition;
    sf::Vector2f size = computeSize();
    if (mHAlignment == HAlignment::Center)
        offset.x += mSize.x * 0.5f - size.x * 0.5f;
    else if (mHAlignment == HAlignment::Right)
        offset.x += mSize.x - size.x;
    for (GuiWidget* widget : mWidgets)
    {
        offset.y = mPosition.y;
        if (mVAlignment == VAlignment::Center)
            offset.y += mSize.y * 0.5f - widget->getSize().y * 0.5f;
        else if (mVAlignment == VAlignment::Bottom)
            offset.y += mSize.y - widget->getSize().y;
        widget->setPosition(offset);
        offset.x += widget->getSize().x + mSpacing;
    }
}

sf::Vector2f GuiHBoxLayout::computeSize() const
{
    sf::Vector2f size;
    for (GuiWidget* widget : mWidgets)
    {
        size.x += widget->getSize().x + mSpacing;
        size.y = std::max(size.y, widget->getSize().y);
    }
    size.x -= mSpacing;
    return size;
}
