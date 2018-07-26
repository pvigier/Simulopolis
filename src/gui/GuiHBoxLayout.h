#pragma once

#include "gui/GuiBoxLayout.h"

class GuiHBoxLayout : public GuiBoxLayout
{
public:
    using GuiBoxLayout::GuiBoxLayout;

    virtual void align() override;
    virtual sf::Vector2f computeSize() const override;
};
