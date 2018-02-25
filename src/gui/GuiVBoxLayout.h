#pragma once

#include "gui/GuiBoxLayout.h"

class GuiVBoxLayout : public GuiBoxLayout
{
public:
    GuiVBoxLayout(HAlignment hAlignment = HAlignment::Left, VAlignment vAlignment = VAlignment::Top);
    virtual ~GuiVBoxLayout();

    virtual void align() override;
    virtual sf::Vector2f computeSize() const override;
};
