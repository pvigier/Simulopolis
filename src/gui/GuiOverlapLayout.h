#pragma once

#include "gui/GuiLayout.h"

class GuiOverlapLayout : public GuiLayout
{
public:
    GuiOverlapLayout();
    GuiOverlapLayout(const PropertyList& properties);
    virtual ~GuiOverlapLayout();

    virtual void align() override;
    virtual sf::Vector2f computeSize() const override;
};
