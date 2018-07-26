#include "gui/GuiLayout.h"
#include "resource/PropertyList.h"

GuiLayout::GuiLayout(Margins margins) : mOwner(nullptr), mMargins(margins)
{
    //ctor
}

GuiLayout::GuiLayout(const PropertyList& properties) : mOwner(nullptr)
{
    mMargins = properties.get<Margins>("margins", Margins{0.0f, 0.0f, 0.0f, 0.0f});
}

GuiLayout::~GuiLayout()
{
    //dtor
}

void GuiLayout::setOwner(GuiWidget* owner)
{
    mOwner = owner;
}

void GuiLayout::setMargins(Margins margins)
{
    mMargins = margins;
}
