#pragma once

#include "gui/GuiBoxLayout.h"

class GuiHBoxLayout : public GuiBoxLayout
{
public:
    GuiHBoxLayout();
    virtual ~GuiHBoxLayout();

    void setAlignment(HAlignment alignment);

protected:
    HAlignment mAlignment;

    virtual void align() override;
};
