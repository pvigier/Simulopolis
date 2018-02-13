#include "gui/GuiHBoxLayout.h"

GuiHBoxLayout::GuiHBoxLayout()
{
    //ctor
}

GuiHBoxLayout::~GuiHBoxLayout()
{
    //dtor
}

void GuiHBoxLayout::setAlignment(HAlignment alignment)
{
    if (alignment != mAlignment)
    {
        mAlignment = alignment;
        update();
    }
}

void GuiHBoxLayout::align()
{

}
