#pragma once

#include "gui/GuiWindow.h"
#include "city/Market.h"
#include "city/Good.h"

class StylesheetManager;
class GuiTable;
class Building;

class GoodsMarketWindow : public GuiWindow
{
public:
    GoodsMarketWindow(StylesheetManager* stylesheetManager, std::array<const Market<const Building>*, 3> markets);
    ~GoodsMarketWindow();

    virtual void setUp() override;

    void onNewMonth();

private:
    StylesheetManager* mStylesheetManager;
    std::array<const Market<const Building>*, 3> mMarkets;
    GuiTable* mTable;

    void addItem(const Building* building, Good good, Money price, int count);
};
