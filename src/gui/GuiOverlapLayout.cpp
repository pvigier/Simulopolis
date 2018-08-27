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
    sf::Vector2f offset = mOwner->getInsidePosition() + sf::Vector2f(mMargins.left, mMargins.top);
    for (GuiWidget* widget : mOwner->getChildren())
        widget->setOutsidePosition(offset);
}

sf::Vector2f GuiOverlapLayout::computeSize() const
{
    sf::Vector2f size;
    for (const GuiWidget* widget : mOwner->getChildren())
    {
        size.x = std::max(size.x, widget->getOutsideSize().x);
        size.y = std::max(size.y, widget->getOutsideSize().y);
    }
    return size + sf::Vector2f(mMargins.left + mMargins.right, mMargins.top + mMargins.bottom);
}
