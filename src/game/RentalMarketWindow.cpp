#include "RentalMarketWindow.h"
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

RentalMarketWindow::RentalMarketWindow(StylesheetManager* stylesheetManager, Market<Lease>* market) :
    GuiWindow("Rental market", stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mMarket(market), mTable(nullptr)
{

}

RentalMarketWindow::~RentalMarketWindow()
{
    //dtor
}

void RentalMarketWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Building", "Owner", "Type", "Rent", "Count"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Window
    add(mTable);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    onNewMonth();
}

void RentalMarketWindow::onNewMonth()
{
    mTable->clear();
    std::map<std::tuple<const Housing*, float>, int> mCounts;
    for (const Market<Lease>::Item* item : mMarket->getItems())
        ++mCounts[std::make_tuple(item->good->getHousing(), item->good->getRent())];
    for (auto it = mCounts.begin(); it != mCounts.end(); ++it)
        addItem(std::get<0>(it->first), std::get<1>(it->first), it->second);
}

void RentalMarketWindow::addItem(const Housing* housing, float rent, int count)
{
    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(format("%d", housing->getId()), 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(housing->getOwner()->getName(), 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(Housing::typeToString(housing->getType()), 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(format("%.2f", rent), 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(format("%d", count), 12, mStylesheetManager->getStylesheet("button")),
    });
}
