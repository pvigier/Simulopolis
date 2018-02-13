#pragma once

#include <vector>
#include "gui/GuiLayout.h"

class GuiBoxLayout : public GuiLayout
{
public:
    GuiBoxLayout();
    virtual ~GuiBoxLayout();

    virtual void update() override;

    virtual sf::FloatRect getRect() const override;

    virtual void addWidget(GuiWidget* widget);
    virtual void setSpacing(unsigned int spacing);

protected:
    std::vector<GuiWidget*> mWidgets;
    unsigned int mSpacing;
};
