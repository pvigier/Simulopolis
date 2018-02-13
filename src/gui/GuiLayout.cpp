#include "gui/GuiLayout.h"

GuiLayout::GuiLayout()
{
    //ctor
}

GuiLayout::~GuiLayout()
{
    //dtor
}

sf::Vector2f GuiLayout::getPosition() const
{
    return mPosition;
}

void GuiLayout::setPosition(sf::Vector2f position)
{
    mPosition = position;
}

sf::Vector2f GuiLayout::getSize() const
{
    return mSize;
}

void GuiLayout::setSize(sf::Vector2f size)
{
    mSize = size;
}
