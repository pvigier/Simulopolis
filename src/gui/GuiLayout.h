#pragma once

#include <vector>
#include "gui/GuiWidget.h"

class GuiLayout
{
public:
    GuiLayout();
    GuiLayout(std::vector<GuiWidget*> widgets);
    virtual ~GuiLayout();

protected:
    std::vector<GuiWidget*> mWidgets;
};
