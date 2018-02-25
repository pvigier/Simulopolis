#pragma once

#include "gui/GuiBoxLayout.h"

class GuiHBoxLayout : public GuiBoxLayout
{
public:
    GuiHBoxLayout(HAlignment hAlignment = HAlignment::Left, VAlignment vAlignment = VAlignment::Top);
    virtual ~GuiHBoxLayout();

    virtual void align() override;
    virtual sf::Vector2f computeSize() const override;
};
