#pragma once

#include "gui/GuiWindow.h"
#include "city/Market.h"

class StylesheetManager;
class GuiTable;
class Lease;
class Housing;

class RentalMarketWindow : public GuiWindow
{
public:
    RentalMarketWindow(StylesheetManager* stylesheetManager, const Market<Lease>* market);
    ~RentalMarketWindow();

    virtual void setUp() override;

    void onNewMonth();

private:
    StylesheetManager* mStylesheetManager;
    const Market<Lease>* mMarket;
    GuiTable* mTable;

    void addItem(const Housing* housing, Money rent, int count);
};
