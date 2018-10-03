/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    std::array<Market<Good>*, 3> markets) :
    GuiWindow("Goods market", stylesheetManager->getStylesheet("window")),
    mMessageBus(messageBus), mStylesheetManager(stylesheetManager), mMarkets(std::move(markets)), mTable(nullptr)
{
    mMessageBus->addMailbox(mMailbox);
    for (Market<Good>* market : mMarkets)
        market->subscribe(mMailbox.getId());
}

GoodsMarketWindow::~GoodsMarketWindow()
{
    for (Market<Good>* market : mMarkets)
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
    applyStyle();

    // Add items
    for (int i = 0; i < 3; ++i)
    {
        for (const Market<Good>::Item* item : mMarkets[i]->getItems())
            addItem(static_cast<MarketBase::Type>(i), item->id);
    }
}

void GoodsMarketWindow::update()
{
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::MARKET)
        {
            const Market<Good>::Event& event = message.getInfo<Market<Good>::Event>();
            switch (event.type)
            {
                case Market<Good>::Event::Type::ITEM_ADDED:
                    addItem(event.marketType, event.itemId);
                    break;
                case Market<Good>::Event::Type::ITEM_REMOVED:
                    removeItem(event.marketType, event.itemId);
                    break;
                default:
                    break;
            }
        }
    }
}

void GoodsMarketWindow::addItem(MarketBase::Type type, Id itemId)
{
    const Market<Good>::Item& item = getMarket(type)->getItem(itemId);
    std::tuple<MarketBase::Type, Id> id(type, itemId);
    mItems[id] = std::make_tuple(item.good->getProductionPlace(), item.good->getType(), item.reservePrice);
    std::size_t i = getRow(mItems[id]);
    if (i == mCounts.size())
    {
        mCounts.emplace_back(mItems[id], 1);
        addRow(item.good->getProductionPlace(), item.good->getType(), item.reservePrice, 1);
    }
    else
        updateRow(i, ++mCounts[i].second);
}

void GoodsMarketWindow::removeItem(MarketBase::Type type, Id itemId)
{
    std::tuple<MarketBase::Type, Id> id(type, itemId);
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

void GoodsMarketWindow::addRow(const Building* building, Good::Type goodType, Money price, int count)
{
    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(building->getOwner()->getName(), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", building->getId()), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(Good::typeToString(goodType), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("$%.2f", price), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", count), 12, mStylesheetManager->getStylesheet("darkText")),
    });
}

void GoodsMarketWindow::updateRow(std::size_t i, int count)
{
    static_cast<GuiText*>(mTable->getCellContent(i, 4))->setString(format("%d", count));
}

Market<Good>* GoodsMarketWindow::getMarket(MarketBase::Type type)
{
    switch (type)
    {
        case MarketBase::Type::NECESSARY_GOOD:
            return mMarkets[0];
        case MarketBase::Type::NORMAL_GOOD:
            return mMarkets[1];
        case MarketBase::Type::LUXURY_GOOD:
            return mMarkets[2];
        default:
            return nullptr;
    }
}
