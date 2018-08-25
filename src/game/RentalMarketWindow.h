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
