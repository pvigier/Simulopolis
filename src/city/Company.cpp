#include "city/Company.h"
#include "city/City.h"
#include "city/Building.h"
#include "city/Housing.h"

Company::Company(std::string name, int creationYear, Person* owner) :
    mName(std::move(name)), mCreationYear(creationYear), mOwner(owner)
{
    sMessageBus->addMailbox(mMailbox);
}

MessageBus* Company::sMessageBus = nullptr;

void Company::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

const std::string& Company::getName() const
{
    return mName;
}

void Company::setCity(City* city)
{
    mCity = city;
}

Person* Company::getOwner() const
{
    return mOwner;
}

void Company::setOwner(Person* owner)
{
    mOwner = owner;
}

const std::vector<Building*>& Company::getBuildings() const
{
    return mBuildings;
}

void Company::addBuilding(Building* building)
{
    building->setOwner(this);
    mBuildings.push_back(building);

    // Add things to markets if necessary
    if (building->isHousing())
    {
        Housing* housing = static_cast<Housing*>(building);
        for (Lease& lease : housing->getLeases())
        {
            if (lease.getTenant() == nullptr)
                addToMarket(lease);
        }
    }
}

void Company::addToMarket(Lease& lease)
{
    Market<Lease>* market = static_cast<Market<Lease>*>(mCity->getMarket(VMarket::Type::RENT));
    market->addItem(mMailbox.getId(), &lease, 0.0f);
}
