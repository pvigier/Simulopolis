#pragma once

#include <unordered_map>
#include <algorithm>
#include "util/IdManager.h"
#include "message/MessageBus.h"
#include "city/Money.h"

class VMarket
{
public:
    enum class Type : int {
        // Goods
        NECESSARY_GOOD = 0,
        NORMAL_GOOD,
        LUXURY_GOOD,
        // Rents
        RENT,
        // Works
        WORK,
        // Count
        COUNT
    };

    VMarket(Type type);
    virtual ~VMarket();

    static void setMessageBus(MessageBus* messageBus);

    virtual void update() = 0;
    virtual void sellItems() = 0;

    Id getMailboxId() const;

protected:
    static MessageBus* sMessageBus;

    Mailbox mMailbox;
    unsigned int mTime;
    Type mType;
};

template<typename T>
class Market : public VMarket
{
public:
    struct Item
    {
        Id id;
        Id sellerId;
        Id sellerAccount;
        T* good;
        Money reservePrice;
    };

    struct Bid
    {
        Id bidderId;
        Money value;
    };

    struct Auction
    {
        unsigned int timestamp;
        Item item;
        std::vector<Bid> bids;
    };

    struct Event
    {
        enum class Type{ADD_ITEM, BID, SET_QUANTITY, PURCHASE, SALE};

        struct AddItemEvent
        {
            Id sellerAccount;
            T* good;
            Money reservePrice;
        };

        struct BidEvent
        {
            Id itemId;
            Money value;
        };

        struct SaleEvent
        {
            Id sellerAccount;
            T* good;
            Money value;
        };

        VMarket::Type marketType;
        Type type;

        union
        {
            AddItemEvent item;
            BidEvent bid;
            unsigned int desiredQuantity;
            SaleEvent sale;
        };
    };

    Market(Type type) : VMarket(type), mDirty(false)
    {

    }

    Id addItem(Id sellerId, Id sellerAccount, T* good, Money reservePrice)
    {
        Auction auction{mTime++, Item{UNDEFINED, sellerId, sellerAccount, good, reservePrice}, {}};
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

    void addBid(Id itemId, Id bidderId, Money value)
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
        while (!mMailbox.isEmpty())
        {
            Message message = mMailbox.get();
            if (message.type == MessageType::MARKET)
            {
                const Event& event = message.getInfo<Event>();
                switch (event.type)
                {
                    case Event::Type::ADD_ITEM:
                        addItem(message.sender, event.item.sellerAccount, event.item.good, event.item.reservePrice);
                        break;
                    case Event::Type::BID:
                        addBid(event.bid.itemId, message.sender, event.bid.value);
                        break;
                    case Event::Type::SET_QUANTITY:
                        setDesiredQuantity(message.sender, event.desiredQuantity);
                    default:
                        break;
                }
            }
        }
    }

    virtual void sellItems() override
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
            if (!auction->bids.empty())
            {
                Bid& bid = *std::max_element(auction->bids.begin(), auction->bids.end(), [&](Bid& lhs, Bid& rhs) { return (mDesiredQuantities[lhs.bidderId] == 0 || (mDesiredQuantities[rhs.bidderId] != 0 && lhs.value < rhs.value)); });
                Item& item = auction->item;
                if (bid.value >= item.reservePrice && mDesiredQuantities[bid.bidderId] > 0)
                {
                    Event event{mType, Event::Type::PURCHASE, {}};
                    event.sale = typename Event::SaleEvent{item.sellerAccount, item.good, bid.value};
                    sMessageBus->send(Message::create(bid.bidderId, MessageType::MARKET, event));
                    sMessageBus->send(Message::create(item.sellerId, MessageType::MARKET, event));
                    mDesiredQuantities[bid.bidderId]--;
                    soldItems.push_back(item.id);
                }
            }
        }

        // Update mActions
        for (Id id : soldItems)
            mAuctions.erase(id);
        if (!soldItems.empty())
            mDirty = true;

        // Reset bids and desired quantities
        for (Auction& auction : mAuctions.getObjects())
            auction.bids.clear();
        mDesiredQuantities.clear();
    }

    Event createAddItemEvent(Id sellerAccount, T* good, Money reservePrice) const
    {
        Event event{mType, Event::Type::ADD_ITEM, {}};
        event.item = typename Event::AddItemEvent{sellerAccount, good, reservePrice};
        return event;
    }

    Event createBidEvent(Id itemId, Money value) const
    {
        Event event{mType, Event::Type::BID, {}};
        event.bid = typename Event::BidEvent{itemId, value};
        return event;
    }

    Event createSetQuantityEvent(unsigned int desiredQuantity) const
    {
        Event event{mType, Event::Type::SET_QUANTITY, {}};
        event.desiredQuantity = desiredQuantity;
        return event;
    }

private:
    IdManager<Auction> mAuctions;
    std::unordered_map<Id, unsigned int> mDesiredQuantities;
    mutable std::vector<const Item*> mItems;
    mutable bool mDirty;
};
