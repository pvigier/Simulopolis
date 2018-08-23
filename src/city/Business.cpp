#include "city/Business.h"
#include "message/MessageBus.h"
#include "city/City.h"
#include "city/Company.h"
#include "city/Market.h"

Business::Business(const std::string& name, Type type, unsigned int nbStairs, Good good, std::size_t maxSizeStock,
    std::size_t nbEmployees, Work::Type employeeType) :
    Building(name, type, nbStairs), mGood(good), mMaxSizeStock(maxSizeStock), mPrice(0.0)
{
    mEmployees.push_back(Work(Work::Type::MANAGER, this));
    mEmployees.resize(nbEmployees + 1, Work(employeeType, this));
}

Business::~Business()
{
    //dtor
}

std::unique_ptr<Tile> Business::clone() const
{
    return std::unique_ptr<Tile>(new Business(mTextureName, mType, mNbStairs, mGood, mMaxSizeStock, mEmployees.size() - 1, mEmployees.back().getType()));
}

void Business::setOwner(Company* owner)
{
    Building::setOwner(owner);
    for (Work& employee : mEmployees)
        employee.setEmployer(mOwner);
    // Register mailbox
    if (mMailbox.getId() == UNDEFINED)
        mOwner->getMessageBus()->addMailbox(mMailbox);
}

void Business::update()
{
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::MARKET)
        {
            const Market<const Building>::Event& event = message.getInfo<Market<const Building>::Event>();
            if (event.type == Market<const Building>::Event::Type::PURCHASE)
            {
                // To do : save the building to fetch the good later
                mStock.push(event.sale.value);
                mOwner->getMessageBus()->send(Message::create(mOwner->getCity()->getBank().getMailboxId(), MessageType::BANK, mOwner->getCity()->getBank().createTransferMoneyEvent(mOwner->getAccount(), event.sale.sellerAccount, event.sale.value)));
            }
        }
        else if (message.type == MessageType::BUSINESS)
        {
            const Event& event = message.getInfo<Event>();
            if (event.type == Event::Type::RESERVATION)
            {
                if (!mStock.empty())
                {
                    mOwner->getMessageBus()->send(Message::create(mMailbox.getId(), message.sender, MessageType::BUSINESS, Event{Event::Type::RESERVATION_ACCEPTED}));
                    mStock.pop();
                }
                else
                    mOwner->getMessageBus()->send(Message::create(mMailbox.getId(), message.sender, MessageType::BUSINESS, Event{Event::Type::RESERVATION_REFUSED}));
            }
        }
    }
}

Good Business::getGood() const
{
    return mGood;
}

std::size_t Business::getStockSize() const
{
    return mStock.size();
}

bool Business::hasPreparedGoods() const
{
    return mPreparedGoods >= 1.0;
}

Money Business::getPrice() const
{
    return mPrice;
}

Work& Business::getManager()
{
    return mEmployees[0];
}

const Work& Business::getManager() const
{
    return mEmployees[0];
}

std::vector<Work>& Business::getEmployees()
{
    return mEmployees;
}

const std::vector<Work>& Business::getEmployees() const
{
    return mEmployees;
}

void Business::onNewMonth()
{
    prepareGoods();
    buyGoods();
}

void Business::prepareGoods()
{
    double preparedGoods = 0.0;
    for (std::size_t i = 1; i < mEmployees.size(); ++i)
    {
        if (mEmployees[i].hasWorkedThisMonth())
            preparedGoods += 1.0;
    }
    if (getManager().hasWorkedThisMonth())
        preparedGoods *= 2.0;
    mPreparedGoods += preparedGoods;
}

void Business::buyGoods()
{
    const Market<const Building>* market = getMarket();
    mOwner->getMessageBus()->send(Message::create(mMailbox.getId(), market->getMailboxId(), MessageType::MARKET, market->createSetQuantityEvent(mMaxSizeStock - mStock.size())));
    for (const Market<const Building>::Item* item : market->getItems())
    {
        if (mOwner->getCity()->getMap().isReachableFrom(this, item->good))
            mOwner->getMessageBus()->send(Message::create(mMailbox.getId(), market->getMailboxId(), MessageType::MARKET, market->createBidEvent(item->id, item->reservePrice)));
    }
}

const Market<const Building>* Business::getMarket()
{
    switch (mGood)
    {
        case Good::NECESSARY:
            return static_cast<const Market<const Building>*>(mOwner->getCity()->getMarket(VMarket::Type::NECESSARY_GOOD));
        case Good::NORMAL:
            return static_cast<const Market<const Building>*>(mOwner->getCity()->getMarket(VMarket::Type::NORMAL_GOOD));
        case Good::LUXURY:
            return static_cast<const Market<const Building>*>(mOwner->getCity()->getMarket(VMarket::Type::LUXURY_GOOD));
        default:
            return nullptr;
    };
}
