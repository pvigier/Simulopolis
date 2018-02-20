#include "gui/GuiBoxLayout.h"

GuiBoxLayout::GuiBoxLayout() : mSpacing(0), mHAlignment(HAlignment::Left), mVAlignment(VAlignment::Top)
{
    //ctor
}

GuiBoxLayout::~GuiBoxLayout()
{
    //dtor
}

void GuiBoxLayout::setSpacing(float spacing)
{
    mSpacing = spacing;
}

void GuiBoxLayout::setHAlignment(HAlignment alignment)
{
    mHAlignment = alignment;
}

void GuiBoxLayout::setVAlignment(VAlignment alignment)
{
    mVAlignment = alignment;
}
