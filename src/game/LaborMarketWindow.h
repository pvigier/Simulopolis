#pragma once

#include "message/Mailbox.h"
#include "gui/GuiWindow.h"
#include "city/Market.h"
#include "city/Work.h"

class MessageBus;
class StylesheetManager;
class GuiTable;
class Building;

class LaborMarketWindow : public GuiWindow
{
public:
    LaborMarketWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager, Market<Work>* market);
    ~LaborMarketWindow();

    virtual void setUp() override;

    void update();

private:
    using Key = std::tuple<const Building*, Work::Type, Money>;

    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    Market<Work>* mMarket;
    std::map<Id, Key> mItems;
    std::vector<std::pair<Key, int>> mCounts;
    GuiTable* mTable;

    // Items
    void addItem(Id itemId);
    void removeItem(Id itemId);

    // Rows
    std::size_t getRow(const Key& key) const;
    void addRow(const Building* building, Work::Type type, Money salary, int count);
    void updateRow(std::size_t i, int count);
};
