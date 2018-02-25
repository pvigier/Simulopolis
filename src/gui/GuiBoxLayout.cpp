#include "gui/GuiBoxLayout.h"

GuiBoxLayout::GuiBoxLayout(HAlignment hAlignment, VAlignment vAlignment) : mSpacing(0),
    mHAlignment(hAlignment), mVAlignment(vAlignment)
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
