#include "gui/GuiLayout.h"

GuiLayout::GuiLayout() : mOwner(nullptr)
{
    //ctor
}

GuiLayout::~GuiLayout()
{
    //dtor
}

void GuiLayout::setOwner(GuiWidget* owner)
{
    mOwner = owner;
}
