#pragma once

#include "gui/GuiWindow.h"
#include "city/Market.h"

class StylesheetManager;
class GuiTable;
class Lease;

class RentalMarketWindow : public GuiWindow
{
public:
    RentalMarketWindow(StylesheetManager* stylesheetManager, Market<Lease>* market);

    ~RentalMarketWindow();

    virtual void setUp() override;

    void onNewMonth();

private:
    StylesheetManager* mStylesheetManager;
    Market<Lease>* mMarket;
    GuiTable* mTable;

    void addItem(const Market<Lease>::Item* item);
};
