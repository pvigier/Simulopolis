#include "GuiLayout.h"

GuiLayout::GuiLayout()
{
    //ctor
}

GuiLayout::GuiLayout(std::vector<GuiWidget*> widgets) : mWidgets(std::move(widgets))
{
    //ctor
}

GuiLayout::~GuiLayout()
{
    //dtor
}
