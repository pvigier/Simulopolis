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
    else
    {
        sf::Vector2i entryPoint;
        sf::Vector2i housingCoords = mOwner->getHome()->getHousing()->getCoordinates();
        sf::Vector2i roadCoords;
        if (mOwner->getCity()->getMap().getNetwork().getAdjacentRoad(housingCoords.y, housingCoords.x, roadCoords) &&
            mOwner->getCity()->getMap().getNetwork().getRandomEntryPoint(roadCoords.y, roadCoords.x, entryPoint))
        {
                // Set position
                mOwner->getCar().getKinematic().setPosition(mOwner->getCity()->getMap().computePosition(entryPoint.y, entryPoint.x) + sf::Vector2f(Tile::SIZE, Tile::SIZE * 0.5f));
                // Terminate
                mState = State::COMPLETED;
        }
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
            return true;
        }
    }
    return false;
}

std::string GoalEnterCity::toString() const
{
    return "Enter the city";
}
