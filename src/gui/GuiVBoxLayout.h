#pragma once

#include "gui/GuiBoxLayout.h"

class GuiVBoxLayout : GuiBoxLayout
{
public:
    GuiVBoxLayout();
    virtual ~GuiVBoxLayout();

    virtual sf::Vector2f getSize() const override;

    void setAlignment(VAlignment alignment);

protected:
    VAlignment mAlignment;

    virtual void align() override;
};
