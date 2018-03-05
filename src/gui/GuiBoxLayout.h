#pragma once

#include "gui/GuiLayout.h"

class PropertyList;

class GuiBoxLayout : public GuiLayout
{
public:
    GuiBoxLayout(HAlignment hAlignment = HAlignment::Left, VAlignment vAlignment = VAlignment::Top);
    GuiBoxLayout(const PropertyList& properties);
    virtual ~GuiBoxLayout();

    virtual void setSpacing(float spacing);
    void setHAlignment(HAlignment alignment);
    void setVAlignment(VAlignment alignment);

protected:
    float mSpacing;
    HAlignment mHAlignment;
    VAlignment mVAlignment;
};
