#include "gui/GuiHBoxLayout.h"
#include "gui/GuiWidget.h"

GuiHBoxLayout::GuiHBoxLayout(HAlignment hAlignment, VAlignment vAlignment, float spacing) :
    GuiBoxLayout(hAlignment, vAlignment, spacing)
{
    //ctor
}

GuiHBoxLayout::GuiHBoxLayout(const PropertyList& properties) : GuiBoxLayout(properties)
{

}

GuiHBoxLayout::~GuiHBoxLayout()
{
    //dtor
}

void GuiHBoxLayout::align()
{
    sf::Vector2f offset = mOwner->getPosition() + sf::Vector2f(mMargins.left, mMargins.top);
    sf::Vector2f size = computeSize();
    if (mHAlignment == HAlignment::Center)
        offset.x += mOwner->getSize().x * 0.5f - size.x * 0.5f;
    else if (mHAlignment == HAlignment::Right)
        offset.x += mOwner->getSize().x - size.x;
    for (GuiWidget* widget : mOwner->getChildren())
    {
        offset.y = mOwner->getPosition().y + mMargins.top;
        if (mVAlignment == VAlignment::Center)
            offset.y += (mOwner->getSize().y - mMargins.top - mMargins.bottom) * 0.5f - widget->getSize().y * 0.5f;
        else if (mVAlignment == VAlignment::Bottom)
            offset.y += mOwner->getSize().y - mMargins.top - mMargins.bottom - widget->getSize().y;
        widget->setPosition(offset);
        offset.x += widget->getSize().x + mSpacing;
    }
}

sf::Vector2f GuiHBoxLayout::computeSize() const
{
    sf::Vector2f size;
    for (const GuiWidget* widget : mOwner->getChildren())
    {
        size.x += widget->getSize().x + mSpacing;
        size.y = std::max(size.y, widget->getSize().y);
    }
    size.x -= mSpacing;
    return size + sf::Vector2f(mMargins.left + mMargins.right, mMargins.top + mMargins.bottom);
}
