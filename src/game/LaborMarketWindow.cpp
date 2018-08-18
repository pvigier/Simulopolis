#include "LaborMarketWindow.h"
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

LaborMarketWindow::LaborMarketWindow(StylesheetManager* stylesheetManager, const Market<Work>* market) :
    GuiWindow("Labor market", stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mMarket(market), mTable(nullptr)
{

}

LaborMarketWindow::~LaborMarketWindow()
{
    //dtor
}

void LaborMarketWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Building", "Company", "Type", "Salary", "Count"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Window
    add(mTable);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    onNewMonth();
}

void LaborMarketWindow::onNewMonth()
{
    mTable->clear();
    std::map<std::tuple<const Building*, Work::Type, Money>, int> mCounts;
    for (const Market<Work>::Item* item : mMarket->getItems())
        ++mCounts[std::make_tuple(item->good->getWorkplace(), item->good->getType(), item->good->getSalary())];
    for (auto it = mCounts.begin(); it != mCounts.end(); ++it)
        addItem(std::get<0>(it->first), std::get<1>(it->first), std::get<2>(it->first), it->second);
}

void LaborMarketWindow::addItem(const Building* building, Work::Type type, Money salary, int count)
{
    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(format("%d", building->getId()), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(building->getOwner()->getName(), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(Work::typeToString(type), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%.2f", salary), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", count), 12, mStylesheetManager->getStylesheet("darkText")),
    });
}
