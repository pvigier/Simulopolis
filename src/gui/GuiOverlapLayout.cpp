#include "gui/GuiOverlapLayout.h"
#include "gui/GuiWidget.h"

GuiOverlapLayout::GuiOverlapLayout()
{
    //ctor
}

GuiOverlapLayout::GuiOverlapLayout(const PropertyList& properties) : GuiLayout(properties)
{

}

GuiOverlapLayout::~GuiOverlapLayout()
{
    //dtor
}

void GuiOverlapLayout::align()
{
    sf::Vector2f offset = mOwner->getPosition() + sf::Vector2f(mMargins.left, mMargins.top);
    for (GuiWidget* widget : mOwner->getChildren())
        widget->setPosition(offset);
}

sf::Vector2f GuiOverlapLayout::computeSize() const
{
    sf::Vector2f size;
    for (const GuiWidget* widget : mOwner->getChildren())
    {
        size.x = std::max(size.x, widget->getSize().x);
        size.y = std::max(size.y, widget->getSize().y);
    }
    return size + sf::Vector2f(mMargins.left + mMargins.right, mMargins.top + mMargins.bottom);
}
