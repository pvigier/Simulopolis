#include "GoodsMarketWindow.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiTable.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "city/Company.h"
#include "city/Housing.h"
#include "util/format.h"

GoodsMarketWindow::GoodsMarketWindow(StylesheetManager* stylesheetManager, std::array<const Market<const Building>*, 3> markets) :
    GuiWindow("Goods market", stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mMarkets(std::move(markets)), mTable(nullptr)
{

}

GoodsMarketWindow::~GoodsMarketWindow()
{
    //dtor
}

void GoodsMarketWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Building", "Company", "Good", "Price", "Count"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Window
    add(mTable);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    onNewMonth();
}

void GoodsMarketWindow::onNewMonth()
{
    mTable->clear();
    std::map<std::tuple<const Building*, Good, Money>, int> mCounts;
    for (int i = 0; i < 3; ++i)
    {
        const Market<const Building>* market = mMarkets[i];
        Good good = static_cast<Good>(i);
        for (const Market<const Building>::Item* item : market->getItems())
            ++mCounts[std::make_tuple(item->good, good, item->reservePrice)];
        for (auto it = mCounts.begin(); it != mCounts.end(); ++it)
            addItem(std::get<0>(it->first), std::get<1>(it->first), std::get<2>(it->first), it->second);
    }
}

void GoodsMarketWindow::addItem(const Building* building, Good good, Money price, int count)
{
    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(format("%d", building->getId()), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(building->getOwner()->getName(), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(goodToString(good), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("$%.2f", price), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", count), 12, mStylesheetManager->getStylesheet("darkText")),
    });
}
