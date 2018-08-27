#include "GoodsMarketWindow.h"
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

GoodsMarketWindow::GoodsMarketWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager,
    std::array<Market<const Building>*, 3> markets) :
    GuiWindow("Goods market", stylesheetManager->getStylesheet("window")),
    mMessageBus(messageBus), mStylesheetManager(stylesheetManager), mMarkets(std::move(markets)), mTable(nullptr)
{
    mMessageBus->addMailbox(mMailbox);
    for (Market<const Building>* market : mMarkets)
        market->subscribe(mMailbox.getId());
}

GoodsMarketWindow::~GoodsMarketWindow()
{
    for (Market<const Building>* market : mMarkets)
        market->unsubscribe(mMailbox.getId());
    mMessageBus->removeMailbox(mMailbox);
}

void GoodsMarketWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Company", "Building", "Good", "Price", "Count"};
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
    for (int i = 0; i < 3; ++i)
    {
        for (const Market<const Building>::Item* item : mMarkets[i]->getItems())
            addItem(static_cast<VMarket::Type>(i), item->id);
    }
}

void GoodsMarketWindow::update()
{
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::MARKET)
        {
            const Market<const Building>::Event& event = message.getInfo<Market<const Building>::Event>();
            switch (event.type)
            {
                case Market<const Building>::Event::Type::ITEM_ADDED:
                    addItem(event.marketType, event.itemId);
                    break;
                case Market<const Building>::Event::Type::ITEM_REMOVED:
                    removeItem(event.marketType, event.itemId);
                    break;
                default:
                    break;
            }
        }
    }
}

void GoodsMarketWindow::addItem(VMarket::Type type, Id itemId)
{
    const Market<const Building>::Item& item = getMarket(type)->getItem(itemId);
    std::tuple<VMarket::Type, Id> id(type, itemId);
    Good good = getGood(type);
    mItems[id] = std::make_tuple(item.good, good, item.reservePrice);
    std::size_t i = getRow(mItems[id]);
    if (i == mCounts.size())
    {
        mCounts.push_back(std::make_pair(mItems[id], 1));
        addRow(item.good, good, item.reservePrice, 1);
    }
    else
        updateRow(i, ++mCounts[i].second);
}

void GoodsMarketWindow::removeItem(VMarket::Type type, Id itemId)
{
    std::tuple<VMarket::Type, Id> id(type, itemId);
    std::size_t i = getRow(mItems[id]);
    --mCounts[i].second;
    if (mCounts[i].second > 0)
        updateRow(i, mCounts[i].second);
    else
    {
        mItems.erase(id);
        mCounts.erase(mCounts.begin() + i);
        mTable->removeRow(i);
    }
}

std::size_t GoodsMarketWindow::getRow(const Key& key) const
{
    return std::find_if(mCounts.begin(), mCounts.end(), [&key](const std::pair<Key, int>& x){ return x.first == key; }) - mCounts.begin();
}

void GoodsMarketWindow::addRow(const Building* building, Good good, Money price, int count)
{
    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(building->getOwner()->getName(), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", building->getId()), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(goodToString(good), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("$%.2f", price), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", count), 12, mStylesheetManager->getStylesheet("darkText")),
    });
}

void GoodsMarketWindow::updateRow(std::size_t i, int count)
{
    static_cast<GuiText*>(mTable->getCellContent(i, 4))->setString(format("%d", count));
}

Market<const Building>* GoodsMarketWindow::getMarket(VMarket::Type type)
{
    switch (type)
    {
        case VMarket::Type::NECESSARY_GOOD:
            return mMarkets[0];
        case VMarket::Type::NORMAL_GOOD:
            return mMarkets[1];
        case VMarket::Type::LUXURY_GOOD:
            return mMarkets[2];
        default:
            return nullptr;
    }
}

Good GoodsMarketWindow::getGood(VMarket::Type type) const
{
    return static_cast<Good>(type);
}
