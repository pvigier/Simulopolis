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

#pragma once

#include "message/Mailbox.h"
#include "gui/GuiWindow.h"
#include "city/Market.h"
#include "city/Good.h"

class MessageBus;
class StylesheetManager;
class GuiTable;
class Building;

class GoodsMarketWindow : public GuiWindow
{
public:
    GoodsMarketWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager, std::array<Market<Good>*, 3> markets);
    virtual ~GoodsMarketWindow();

    virtual void setUp() override;

    void update();

private:
    using Key = std::tuple<const Building*, GoodType, Money>;

    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    std::array<Market<Good>*, 3> mMarkets;
    std::map<std::tuple<MarketType, Id>, Key> mItems;
    std::vector<std::pair<Key, int>> mCounts;
    GuiTable* mTable;

    // Items
    void addItem(MarketType type, Id itemId);
    void removeItem(MarketType type, Id itemId);

    // Rows
    std::size_t getRow(const Key& key) const;
    void addRow(const Building* building, GoodType goodType, Money price, int count);
    void updateRow(std::size_t i, int count);

    // Util
    Market<Good>* getMarket(MarketType type);
};
