#pragma once

#include "util/IdManager.h"
#include "message/MessageBus.h"

template<typename T, int N>
class Market
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
        float value;
        const T* good;
        int goodType;
    };

    Market() : mTime(0)
    {

    }

    static void setMessageBus(MessageBus* messageBus)
    {
        sMessageBus = messageBus;
    }

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

    void update()
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
                sMessageBus->send(Message::create(bid.bidderId, MessageType::MARKET, Event{Event::Type::PURCHASE, bid.value, item.good, N}));
                sMessageBus->send(Message::create(item.sellerId, MessageType::MARKET, Event{Event::Type::SALE, bid.value, item.good, N}));
                mDesiredQuantities[bid.bidderId]--;
                soldItems.push_back(item.id);
            }
        }
        // Update mActions
        for (Id id : soldItems)
            mAuctions.erase(id);
    }

private:
    static MessageBus* sMessageBus;

    unsigned int mTime;
    IdManager<Auction> mAuctions;
    std::unordered_map<Id, unsigned int> mDesiredQuantities;
    mutable std::vector<const Item*> mItems;
    mutable bool mDirty;
};

template<typename T, int N>
MessageBus* Market<T, N>::sMessageBus = nullptr;
