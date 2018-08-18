#pragma once

#include "gui/GuiWindow.h"
#include "city/Market.h"
#include "city/Work.h"

class StylesheetManager;
class GuiTable;
class Building;

class LaborMarketWindow : public GuiWindow
{
public:
    LaborMarketWindow(StylesheetManager* stylesheetManager, const Market<Work>* market);
    ~LaborMarketWindow();

    virtual void setUp() override;

    void onNewMonth();

private:
    StylesheetManager* mStylesheetManager;
    const Market<Work>* mMarket;
    GuiTable* mTable;

    void addItem(const Building* building, Work::Type type, Money salary, int count);
};
