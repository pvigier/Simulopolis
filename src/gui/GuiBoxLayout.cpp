#include "gui/GuiBoxLayout.h"

GuiBoxLayout::GuiBoxLayout() : mSpacing(0), mHAlignment(HAlignment::Left), mVAlignment(VAlignment::Top)
{
    //ctor
}

GuiBoxLayout::~GuiBoxLayout()
{
    //dtor
}

void GuiBoxLayout::update()
{
    // Align
    align();
    // Update the children
    for (GuiWidget* widget : mWidgets)
        widget->update();
}

sf::FloatRect GuiBoxLayout::getRect() const
{
    return sf::FloatRect(mPosition, getSize());
}

void GuiBoxLayout::onHover(sf::Vector2f position)
{
    for (GuiWidget* widget : mWidgets)
        widget->updateMouseMoved(position);
}

void GuiBoxLayout::onPress(sf::Vector2f position)
{
    for (GuiWidget* widget : mWidgets)
        widget->updateMouseButtonPressed(position);
}

void GuiBoxLayout::onRelease(sf::Vector2f position)
{
    for (GuiWidget* widget : mWidgets)
        widget->updateMouseButtonReleased(position);
}

void GuiBoxLayout::add(GuiWidget* widget)
{
    mWidgets.push_back(widget);
    update();
}

void GuiBoxLayout::setSpacing(float spacing)
{
    mSpacing = spacing;
}

void GuiBoxLayout::setHAlignment(HAlignment alignment)
{
    if (alignment != mHAlignment)
    {
        mHAlignment = alignment;
        update();
    }
}

void GuiBoxLayout::setVAlignment(VAlignment alignment)
{
    if (alignment != mVAlignment)
    {
        mVAlignment = alignment;
        update();
    }
}

void GuiBoxLayout::fitSizeToContent()
{
    mSize = computeSize();
    update();
}

void GuiBoxLayout::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const GuiWidget* widget : mWidgets)
        target.draw(*widget);
}
