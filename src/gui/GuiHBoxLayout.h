#pragma once

#include "gui/GuiBoxLayout.h"

class GuiHBoxLayout : public GuiBoxLayout
{
public:
    GuiHBoxLayout();
    virtual ~GuiHBoxLayout();

protected:
    virtual void align() override;
    virtual sf::Vector2f computeSize() const override;
};
