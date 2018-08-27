#include "LaborMarketWindow.h"
#include "message/MessageBus.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiTable.h"
#include "gui/GuiScrollArea.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "city/Company.h"
#include "city/Housing.h"
#include "util/format.h"

LaborMarketWindow::LaborMarketWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager,
    Market<Work>* market) :
    GuiWindow("Labor market", stylesheetManager->getStylesheet("window")),
    mMessageBus(messageBus), mStylesheetManager(stylesheetManager), mMarket(market), mTable(nullptr)
{
    mMessageBus->addMailbox(mMailbox);
    mMarket->subscribe(mMailbox.getId());
}

LaborMarketWindow::~LaborMarketWindow()
{
    mMarket->unsubscribe(mMailbox.getId());
    mMessageBus->removeMailbox(mMailbox);
}

void LaborMarketWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Company", "Building", "Type", "Salary", "Count"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Scroll area
    GuiScrollArea* scrollArea = mGui->createWithDefaultName<GuiScrollArea>(sf::Vector2i(400, 200), mStylesheetManager->getStylesheet("scrollarea"));
    scrollArea->add(mTable);
    scrollArea->setLayout(std::make_unique<GuiVBoxLayout>());

    // Window
    add(scrollArea);
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add items
    for (const Market<Work>::Item* item : mMarket->getItems())
        addItem(item->id);
}

void LaborMarketWindow::update()
{
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::MARKET)
        {
            const Market<Work>::Event& event = message.getInfo<Market<Work>::Event>();
            switch (event.type)
            {
                case Market<Work>::Event::Type::ITEM_ADDED:
                    addItem(event.itemId);
                    break;
                case Market<Work>::Event::Type::ITEM_REMOVED:
                    removeItem(event.itemId);
                    break;
                default:
                    break;
            }
        }
    }
}

void LaborMarketWindow::addItem(Id itemId)
{
    const Market<Work>::Item& item = mMarket->getItem(itemId);
    mItems[itemId] = std::make_tuple(item.good->getWorkplace(), item.good->getType(), item.reservePrice);
    std::size_t i = getRow(mItems[itemId]);
    if (i == mCounts.size())
    {
        mCounts.push_back(std::make_pair(mItems[itemId], 1));
        addRow(item.good->getWorkplace(), item.good->getType(), item.reservePrice, 1);
    }
    else
        updateRow(i, ++mCounts[i].second);
}

void LaborMarketWindow::removeItem(Id itemId)
{
    std::size_t i = getRow(mItems[itemId]);
    --mCounts[i].second;
    if (mCounts[i].second > 0)
        updateRow(i, mCounts[i].second);
    else
    {
        mItems.erase(itemId);
        mCounts.erase(mCounts.begin() + i);
        mTable->removeRow(i);
    }
}

std::size_t LaborMarketWindow::getRow(const Key& key) const
{
    return std::find_if(mCounts.begin(), mCounts.end(), [&key](const std::pair<Key, int>& x){ return x.first == key; }) - mCounts.begin();
}

void LaborMarketWindow::addRow(const Building* building, Work::Type type, Money salary, int count)
{
    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(building->getOwner()->getName(), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", building->getId()), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(Work::typeToString(type), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("$%.2f", salary), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", count), 12, mStylesheetManager->getStylesheet("darkText")),
    });
}

void LaborMarketWindow::updateRow(std::size_t i, int count)
{
    static_cast<GuiText*>(mTable->getCellContent(i, 4))->setString(format("%d", count));
}

