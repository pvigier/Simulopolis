#include "ai/GoalEnterCity.h"
#include "ai/GoalMoveTo.h"
#include "city/Person.h"
#include "city/City.h"
#include "city/Market.h"

GoalEnterCity::GoalEnterCity(Person* owner) : Goal(owner), mMarket(nullptr), mHomeFound(false)
{

}

GoalEnterCity::~GoalEnterCity()
{

}

void GoalEnterCity::activate()
{
    mState = State::ACTIVE;
    // Choose the right market
    mMarket = static_cast<Market<Lease>*>(mOwner->getCity()->getMarket(VMarket::Type::RENT));
}

Goal::State GoalEnterCity::process()
{
    activateIfInactive();

    if (!mHomeFound)
    {
        for (const Market<Lease>::Item* item : mMarket->getItems())
            mMarket->addBid(item->id, mOwner->getMailboxId(), 0.0f);
    }

    return mState;
}

void GoalEnterCity::terminate()
{

}

bool GoalEnterCity::handle(Message message)
{
    if (message.type == MessageType::MARKET)
    {
        const Market<Lease>::Event& event = message.getInfo<Market<Lease>::Event>();
        if (event.type == Market<Lease>::Event::Type::PURCHASE)
        {
            mHomeFound = true;
            mOwner->setHome(event.good);
            sf::Vector2i entryPoint;
            if (mOwner->getCity()->getMap().getNetwork().getRandomEntryPoint(entryPoint))
            {
                mOwner->getCar().getKinematic().setPosition(mOwner->getCity()->getMap().computePosition(entryPoint.x, entryPoint.y));
                mOwner->getShortTermBrain().activate();
                mOwner->getShortTermBrain().clearSubgoals();
                mOwner->getShortTermBrain().pushFront(new GoalMoveTo(mOwner, mOwner->getHome()->getHousing()));
            }
            else
                mState = State::FAILED;
            return true;
        }
    }
    return false;
}

std::string GoalEnterCity::toString() const
{
    return "Enter the city";
}
