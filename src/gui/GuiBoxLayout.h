#pragma once

#include "gui/GuiLayout.h"

class GuiBoxLayout : public GuiLayout
{
public:
    GuiBoxLayout();
    virtual ~GuiBoxLayout();

    virtual void setSpacing(float spacing);
    void setHAlignment(HAlignment alignment);
    void setVAlignment(VAlignment alignment);

protected:
    float mSpacing;
    HAlignment mHAlignment;
    VAlignment mVAlignment;
};
