#include "gui/GuiVBoxLayout.h"
#include "gui/GuiWidget.h"

void GuiVBoxLayout::align()
{
    sf::Vector2f offset = mOwner->getInsidePosition() + sf::Vector2f(mMargins.left, mMargins.top);
    sf::Vector2f size = computeSize();
    if (mVAlignment == VAlignment::Center)
        offset.y += mOwner->getInsideSize().y * 0.5f - size.y * 0.5f;
    else if (mVAlignment == VAlignment::Bottom)
        offset.y += mOwner->getInsideSize().y - size.y;
    for (GuiWidget* widget : mOwner->getChildren())
    {
        offset.x = mOwner->getInsidePosition().x + mMargins.left;
        if (mHAlignment == HAlignment::Center)
            offset.x += (mOwner->getInsideSize().x - mMargins.left - mMargins.right) * 0.5f - widget->getOutsideSize().x * 0.5f;
        else if (mHAlignment == HAlignment::Right)
            offset.x += mOwner->getInsideSize().x - mMargins.left - mMargins.right - widget->getOutsideSize().x;
        widget->setOutsidePosition(offset);
        offset.y += widget->getOutsideSize().y + mSpacing;
    }
}

sf::Vector2f GuiVBoxLayout::computeSize() const
{
    sf::Vector2f size;
    for (const GuiWidget* widget : mOwner->getChildren())
    {
        size.x = std::max(size.x, widget->getOutsideSize().x);
        size.y += widget->getOutsideSize().y + mSpacing;
    }
    size.y -= mSpacing;
    return size + sf::Vector2f(mMargins.left + mMargins.right, mMargins.top + mMargins.bottom);
}
