#include "gui/GuiBoxLayout.h"

GuiBoxLayout::GuiBoxLayout()
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

void GuiBoxLayout::addWidget(GuiWidget* widget)
{
    mWidgets.push_back(widget);
    update();
}

void GuiBoxLayout::setSpacing(unsigned int spacing)
{
    mSpacing = spacing;
}
