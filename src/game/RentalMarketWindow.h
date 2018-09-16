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

class MessageBus;
class StylesheetManager;
class GuiTable;
class Lease;
class Housing;

class RentalMarketWindow : public GuiWindow
{
public:
    RentalMarketWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager, Market<Lease>* market);
    ~RentalMarketWindow();

    virtual void setUp() override;

    void update();

private:
    using Key = std::tuple<const Housing*, Money>;

    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    Market<Lease>* mMarket;
    std::map<Id, Key> mItems;
    std::vector<std::pair<Key, int>> mCounts;
    GuiTable* mTable;

    // Items
    void addItem(Id itemId);
    void removeItem(Id itemId);

    // Rows
    std::size_t getRow(const Key& key) const;
    void addRow(const Housing* housing, Money rent, int count);
    void updateRow(std::size_t i, int count);
};
