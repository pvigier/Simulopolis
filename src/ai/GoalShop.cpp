#include "GoalShop.h"
#include "city/Person.h"
#include "city/Housing.h"
#include "city/Business.h"
#include "city/City.h"
#include "city/Lease.h"
#include "GoalMoveTo.h"
#include "GoalWait.h"

GoalShop::GoalShop(Person* owner) : Goal(owner), mSelectedShop(nullptr), mGoodReserved(false)
{
    //ctor
}

GoalShop::~GoalShop()
{
    //dtor
}

void GoalShop::activate()
{
    mState = State::ACTIVE;

    // Select the right type of shop
    Tile::Type type;
    switch (mOwner->getConsumptionHabit())
    {
        case Good::NECESSARY:
            type = Tile::Type::GROCERY;
            break;
        case Good::NORMAL:
            type = Tile::Type::MALL;
            break;
        default:
            type = Tile::Type::BOUTIQUE;
            break;
    }

    // Find a shop
    std::vector<const Building*> buildings = mOwner->getCity()->getMap().getReachableBuildingsAround(mOwner->getHome()->getHousing(), RADIUS, type);
    for (const Building* building : buildings)
    {
        const Business* shop = static_cast<const Business*>(building);
        if (mOwner->getCity()->getMap().isReachableFrom(mOwner->getHome()->getHousing(), shop) &&
            shop->hasPreparedGoods() && shop->getPrice() <= mOwner->getAccountBalance())
        {
            if (!mSelectedShop || shop->getPrice() < mSelectedShop->getPrice())
                mSelectedShop = shop;
        }
    }

    if (mSelectedShop)
    {
        // Reserve a good
        mOwner->getMessageBus()->send(Message::create(mOwner->getMailboxId(), mSelectedShop->getMailboxId(), MessageType::BUSINESS, Business::Event{Business::Event::Type::RESERVATION, {}, {}}));

        // Clear subgoals
        clearSubgoals();
    }
    else
        mState = State::FAILED;
}

Goal::State GoalShop::process()
{
    activateIfInactive();

    if (mGoodReserved)
        mState = processSubgoals();

    return mState;
}

void GoalShop::terminate()
{

}

bool GoalShop::handle(Message message)
{
    if (message.type == MessageType::BUSINESS)
    {
        const Business::Event& event = message.getInfo<Business::Event>();
        if (event.type == Business::Event::Type::RESERVATION_ACCEPTED)
        {
            mGoodReserved = true;
            mOwner->getMessageBus()->send(Message::create(mOwner->getCity()->getBank().getMailboxId(), MessageType::BANK, mOwner->getCity()->getBank().createTransferMoneyEvent(mOwner->getAccount(), event.accountId, event.price)));
            pushBack(new GoalMoveTo(mOwner, mSelectedShop));
            //pushBack(new GoalWait(mOwner, mOwner->getCity()->toHumanTime(4.0f * 1.0f)));
        }
        else if (event.type == Business::Event::Type::RESERVATION_REFUSED)
            mState = State::FAILED;
    }
    return false;
}

std::string GoalShop::toString() const
{
    return "Shop";
}
