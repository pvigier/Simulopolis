#include "gui/GuiVBoxLayout.h"
#include "gui/GuiWidget.h"

GuiVBoxLayout::GuiVBoxLayout(HAlignment hAlignment, VAlignment vAlignment) :
    GuiBoxLayout(hAlignment, vAlignment)
{
    //ctor
}

GuiVBoxLayout::~GuiVBoxLayout()
{
    //dtor
}

void GuiVBoxLayout::align()
{
    sf::Vector2f offset = mOwner->getPosition();
    sf::Vector2f size = computeSize();
    if (mVAlignment == VAlignment::Center)
        offset.y += mOwner->getSize().y * 0.5f - size.y * 0.5f;
    else if (mVAlignment == VAlignment::Bottom)
        offset.y += mOwner->getSize().y - size.y;
    for (GuiWidget* widget : mOwner->getChildren())
    {
        offset.x = mOwner->getPosition().x;
        if (mHAlignment == HAlignment::Center)
            offset.x += mOwner->getSize().x * 0.5f - widget->getSize().x * 0.5f;
        else if (mHAlignment == HAlignment::Right)
            offset.x += mOwner->getSize().x - widget->getSize().x;
        widget->setPosition(offset);
        offset.y += widget->getSize().y + mSpacing;
    }
}

sf::Vector2f GuiVBoxLayout::computeSize() const
{
    sf::Vector2f size;
    for (GuiWidget* widget : mOwner->getChildren())
    {
        size.x = std::max(size.x, widget->getSize().x);
        size.y += widget->getSize().y + mSpacing;
    }
    size.y -= mSpacing;
    return size;
}
