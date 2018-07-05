#pragma once

#include "util/IdManager.h"

template<typename T>
class Market
{
public:
    struct Item
    {
        Id id;
        const T& good;
        float reservePrice;
    };

    struct Bid
    {
        Id bidderId;
        float value;
    };

    struct Auction
    {
        Item item;
        std::vector<Bid> bids;
    };

    Market()
    {

    }

    Id addItem(const T& good, float reservePrice)
    {
        Auction auction{Item{good, reservePrice, UNDEFINED}, {}};
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

    void addBid(Id itemId, Id bidderId, float value)
    {
        mAuctions.get(itemId).bids.emplace_back(bidderId, value);
    }

    const std::vector<Bid>& getBids(Id itemId) const
    {
        return mAuctions.get(itemId).bids;
    }

private:
    IdManager<Auction> mAuctions;
    mutable std::vector<const Item*> mItems;
    mutable bool mDirty;
};
