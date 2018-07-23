#pragma once

#include <unordered_map>
#include <algorithm>
#include "util/IdManager.h"
#include "message/MessageBus.h"

class VMarket
{
public:
    enum class Type : int {
        // Goods
        NECESSARY_GOOD = 0,
        NORMAL_GOOD,
        LUXURY_GOOD,
        // Housing
        AFFORDABLE_HOUSING_RENT,
        APARTMENT_BUILDING_RENT,
        VILLA_RENT,
        // Jobs
        NON_QUALIFIED_JOB,
        QUALIFIED_JOB,
        // Count
        COUNT
    };

    VMarket(Type type);

    static void setMessageBus(MessageBus* messageBus);

    virtual void update() = 0;

protected:
    static MessageBus* sMessageBus;
    unsigned int mTime;
    Type mType;
};

template<typename T = int>
class Market : public VMarket
{
public:
    struct Item
    {
        Id id;
        Id sellerId;
        const T* good;
        float reservePrice;
    };

    struct Bid
    {
        Id bidderId;
        float value;
    };

    struct Auction
    {
        unsigned int timestamp;
        Item item;
        std::vector<Bid> bids;
    };

    struct Event
    {
        enum class Type{PURCHASE, SALE};

        Type type;
        VMarket::Type marketType;
        const T* good;
        float value;
    };

    using VMarket::VMarket;

    Id addItem(Id sellerId, const T* good, float reservePrice)
    {
        Auction auction{mTime++, Item{UNDEFINED, sellerId, good, reservePrice}, {}};
        Id id = mAuctions.add(auction);
        mAuctions.get(id).item.id = id;
        mDirty = true;
        return id;
    }

    const std::vector<const Item*>& getItems() const
    {
        if (mDirty)
        {
            mItems.clear();
            for (const Auction& auction : mAuctions.getObjects())
                mItems.push_back(&auction.item);
            mDirty = false;
        }
        return mItems;
    }

    void setDesiredQuantity(Id bidderId, unsigned int quantity)
    {
        mDesiredQuantities[bidderId] = quantity;
    }

    void addBid(Id itemId, Id bidderId, float value)
    {
        if (mDesiredQuantities.find((bidderId)) == mDesiredQuantities.end())
            mDesiredQuantities[bidderId] = 1;
        mAuctions.get(itemId).bids.push_back(Bid{bidderId, value});
    }

    const std::vector<Bid>& getBids(Id itemId) const
    {
        return mAuctions.get(itemId).bids;
    }

    virtual void update() override
    {
        // Sort auctions by timestamp
        std::vector<Auction*> auctions;
        for (Auction& auction : mAuctions.getObjects())
            auctions.push_back(&auction);
        std::sort(auctions.begin(), auctions.end(), [&](Auction* lhs, Auction* rhs) { return lhs->timestamp < rhs->timestamp; });
        // Sell the goods
        std::vector<Id> soldItems;
        for (Auction* auction : auctions)
        {
            Bid& bid = *std::max_element(auction->bids.begin(), auction->bids.end(), [&](Bid& lhs, Bid& rhs) { return (mDesiredQuantities[lhs.bidderId] == 0 || (mDesiredQuantities[rhs.bidderId] != 0 && lhs.value < rhs.value)); });
            Item& item = auction->item;
            if (bid.value >= item.reservePrice && mDesiredQuantities[bid.bidderId] > 0)
            {
                sMessageBus->send(Message::create(bid.bidderId, MessageType::MARKET, Event{Event::Type::PURCHASE, mType, item.good, bid.value}));
                sMessageBus->send(Message::create(item.sellerId, MessageType::MARKET, Event{Event::Type::SALE, mType, item.good, bid.value}));
                mDesiredQuantities[bid.bidderId]--;
                soldItems.push_back(item.id);
            }
        }
        // Update mActions
        for (Id id : soldItems)
            mAuctions.erase(id);
    }

private:
    IdManager<Auction> mAuctions;
    std::unordered_map<Id, unsigned int> mDesiredQuantities;
    mutable std::vector<const Item*> mItems;
    mutable bool mDirty;
};
