#pragma once

#include "gui/GuiWidget.h"

class GuiTabWidget : public GuiWidget
{
public:
    GuiTabWidget();
    GuiTabWidget(const PropertyList& properties);

    void setCurrentTab(std::size_t currentTab);
};
