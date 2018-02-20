#pragma once

#include "gui/GuiBoxLayout.h"

class GuiVBoxLayout : public GuiBoxLayout
{
public:
    GuiVBoxLayout();
    virtual ~GuiVBoxLayout();

    virtual void align() override;
    virtual sf::Vector2f computeSize() const override;
};
