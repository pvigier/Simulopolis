#include "gui/GuiBoxLayout.h"
#include "resource/PropertyList.h"

GuiBoxLayout::GuiBoxLayout(HAlignment hAlignment, VAlignment vAlignment, float spacing) : mSpacing(spacing),
    mHAlignment(hAlignment), mVAlignment(vAlignment)
{
    //ctor
}

GuiBoxLayout::GuiBoxLayout(const PropertyList& properties)
{
    mHAlignment = properties.get("hAlignment", HAlignment::Left);
    mVAlignment = properties.get("vAlignment", VAlignment::Top);
    mSpacing = properties.get("spacing", 0.0f);
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
