#pragma once

#include "gui/GuiLayout.h"

class PropertyList;

class GuiBoxLayout : public GuiLayout
{
public:
    GuiBoxLayout(HAlignment hAlignment = HAlignment::Left, VAlignment vAlignment = VAlignment::Top,
        float spacing = 0.0f, Margins margins = Margins{0.0f, 0.0f, 0.0f, 0.0f});
    GuiBoxLayout(float spacing, Margins margins = Margins{0.0f, 0.0f, 0.0f, 0.0f});
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
