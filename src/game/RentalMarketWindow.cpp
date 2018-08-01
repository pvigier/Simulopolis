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
    std::vector<std::string> names{"Owner", "Type", "Rent"};
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
    for (const Market<Lease>::Item* item : mMarket->getItems())
        addItem(item);
}

void RentalMarketWindow::addItem(const Market<Lease>::Item* item)
{
    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(item->good->getOwner()->getName(), 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(Housing::typeToString(item->good->getHousing()->getType()), 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(format("%.2f", item->good->getRent()), 12, mStylesheetManager->getStylesheet("button")),
    });
}
