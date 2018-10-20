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

#include <unordered_map>
#include <algorithm>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_member.hpp>
#include "util/IdManager.h"
#include "util/debug.h"
#include "message/MessageBus.h"
#include "message/Subject.h"
#include "city/Money.h"

enum class MarketType : int {
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

class MarketBase : public Subject
{
public:
    struct EventBase
    {
        MarketType marketType;

        EventBase();
        EventBase(MarketType marketType);

    private:
        // Serialization
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & marketType;
        }
    };

    MarketBase(MarketType type);
    virtual ~MarketBase();

    void setMessageBus(MessageBus* messageBus, bool alreadyAdded = false);

    virtual void update() = 0;
    virtual void sellItems() = 0;

    Id getMailboxId() const;

protected:
    MessageBus* mMessageBus;
    Mailbox mMailbox;
    unsigned int mTime;
    MarketType mType;

    MarketBase() = default; // Only for serialization

private:
    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mMailbox & mTime & mType;
    }
};

template<typename T>
class Market : public MarketBase
{
public:
    struct Item
    {
        Id id;
        Id sellerId;
        Id sellerAccount;
        T* good;
        Money reservePrice;

    private:
        // Serialization
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & id & sellerId & sellerAccount & good & reservePrice;
        }
    };

    struct Bid
    {
        Id bidderId;
        Money value;

    private:
        // Serialization
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & bidderId & value;
        }
    };

    struct Auction
    {
        unsigned int timestamp;
        Item item;
        std::vector<Bid> bids;

    private:
        // Serialization
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & timestamp & item & bids;
        }
    };

    struct Event : public EventBase
    {
        enum class Type{ADD_ITEM, REMOVE_ITEM, BID, SET_QUANTITY, PURCHASE, SALE, ITEM_ADDED, ITEM_REMOVED};

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
            Id itemId;
            Id sellerAccount;
            T* good;
            Money value;
        };

        Type type;

        union
        {
            AddItemEvent item;
            BidEvent bid;
            unsigned int desiredQuantity;
            SaleEvent sale;
            Id itemId;
        };

        Event()
        {

        }

        Event(MarketType marketType, Type type) : EventBase(marketType), type(type)
        {

        }

    private:
        // Serialization
        // I'm forced to put the code here because external serialization doesn't work with depdent types.
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & boost::serialization::base_object<MarketBase::EventBase>(*this);
            ar & type;
            switch (type)
            {
                case Market<T>::Event::Type::ADD_ITEM:
                    ar & item.sellerAccount & item.good & item.reservePrice;
                    break;
                case Market<T>::Event::Type::REMOVE_ITEM:
                case Market<T>::Event::Type::ITEM_ADDED:
                case Market<T>::Event::Type::ITEM_REMOVED:
                    ar & itemId;
                    break;
                case Market<T>::Event::Type::BID:
                    ar & bid.itemId & bid.value;
                    break;
                case Market<T>::Event::Type::SET_QUANTITY:
                    ar & desiredQuantity;
                    break;
                case Market<T>::Event::Type::PURCHASE:
                case Market<T>::Event::Type::SALE:
                    ar & sale.itemId & sale.sellerAccount & sale.good & sale.value;
                    break;
            }
        }
    };

    Market(MarketType type) : MarketBase(type), mDirty(false)
    {

    }

    Id addItem(Id sellerId, Id sellerAccount, T* good, Money reservePrice)
    {
        // Create a new auction
        Auction auction{mTime++, Item{UNDEFINED, sellerId, sellerAccount, good, reservePrice}, {}};
        Id id = mAuctions.add(auction);
        mAuctions.get(id).item.id = id;
        mDirty = true;
        // Notify
        Event event = createItemAddedEvent(id);
        mMessageBus->send(Message::create(sellerId, MessageType::MARKET, event));
        notify(Message::create(MessageType::MARKET, event));
        return id;
    }

    void removeItem(Id itemId)
    {
        // Remove item
        mAuctions.erase(itemId);
        mDirty = true;
        // Notify
        notify(Message::create(MessageType::MARKET, createItemRemovedEvent(itemId)));
    }

    const Item& getItem(Id itemId) const
    {
        return mAuctions.get(itemId).item;
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
                        if (message.sender == UNDEFINED)
                            DEBUG(static_cast<int>(mType) << " Add item: sender undefined\n");
                        else
                            addItem(message.sender, event.item.sellerAccount, event.item.good, event.item.reservePrice);
                        break;
                    case Event::Type::REMOVE_ITEM:
                        if (!mAuctions.has(event.itemId))
                        {
                            DEBUG(static_cast<int>(mType) << " Remove item: " << event.itemId << " is not a valid item id\n");
                            DEBUG("items: ");
                            for (const Auction& auction : mAuctions.getObjects())
                                DEBUG(auction.item.id << " ");
                            DEBUG("\n");
                        }
                        else
                            removeItem(event.itemId);
                        break;
                    case Event::Type::BID:
                        if (message.sender == UNDEFINED)
                            DEBUG(static_cast<int>(mType) << " Bid: sender undefined\n");
                        else if (!mAuctions.has(event.bid.itemId))
                            DEBUG(static_cast<int>(mType) << " Bid: " << event.bid.itemId << " is not a valid item id\n");
                        else
                            addBid(event.bid.itemId, message.sender, event.bid.value);
                        break;
                    case Event::Type::SET_QUANTITY:
                        if (message.sender == UNDEFINED)
                            DEBUG(static_cast<int>(mType) << " Set quantity: sender undefined\n");
                        else
                            setDesiredQuantity(message.sender, event.desiredQuantity);
                        break;
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
                    mMessageBus->send(Message::create(bid.bidderId, MessageType::MARKET, createPurchaseEvent(item, bid.value)));
                    mMessageBus->send(Message::create(item.sellerId, MessageType::MARKET, createSaleEvent(item, bid.value)));
                    --mDesiredQuantities[bid.bidderId];
                    soldItems.push_back(item.id);
                }
            }
        }

        // Update mActions
        for (Id id : soldItems)
        {
            mAuctions.erase(id);
            // Notify
            notify(Message::create(MessageType::MARKET, createItemRemovedEvent(id)));
        }
        if (!soldItems.empty())
            mDirty = true;

        // Reset bids and desired quantities
        for (Auction& auction : mAuctions.getObjects())
            auction.bids.clear();
        mDesiredQuantities.clear();
    }

    Event createAddItemEvent(Id sellerAccount, T* good, Money reservePrice) const
    {
        Event event(mType, Event::Type::ADD_ITEM);
        event.item = typename Event::AddItemEvent{sellerAccount, good, reservePrice};
        return event;
    }

    Event createRemoveItemEvent(Id itemId) const
    {
        Event event(mType, Event::Type::REMOVE_ITEM);
        event.itemId = itemId;
        return event;
    }

    Event createBidEvent(Id itemId, Money value) const
    {
        Event event(mType, Event::Type::BID);
        event.bid = typename Event::BidEvent{itemId, value};
        return event;
    }

    Event createSetQuantityEvent(unsigned int desiredQuantity) const
    {
        Event event(mType, Event::Type::SET_QUANTITY);
        event.desiredQuantity = desiredQuantity;
        return event;
    }

    Event createPurchaseEvent(Item& item, Money value) const
    {
        Event event(mType, Event::Type::PURCHASE);
        event.sale = typename Event::SaleEvent{item.id, item.sellerAccount, item.good, value};
        return event;
    }

    Event createSaleEvent(Item& item, Money value) const
    {
        Event event(mType, Event::Type::SALE);
        event.sale = typename Event::SaleEvent{item.id, item.sellerAccount, item.good, value};
        return event;
    }

    Event createItemAddedEvent(Id itemId) const
    {
        Event event(mType, Event::Type::ITEM_ADDED);
        event.itemId = itemId;
        return event;
    }

    Event createItemRemovedEvent(Id itemId) const
    {
        Event event(mType, Event::Type::ITEM_REMOVED);
        event.itemId = itemId;
        return event;
    }

private:
    IdManager<Auction> mAuctions;
    std::unordered_map<Id, unsigned int> mDesiredQuantities;
    mutable std::vector<const Item*> mItems;
    mutable bool mDirty;

    // Serialization
    friend class boost::serialization::access;

    Market() = default;

    template<typename Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
        ar & boost::serialization::base_object<MarketBase>(*this);
        ar & mAuctions & mDesiredQuantities;
    }

    template<typename Archive>
    void load(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<MarketBase>(*this);
        ar & mAuctions & mDesiredQuantities;
        mDirty = true;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};
