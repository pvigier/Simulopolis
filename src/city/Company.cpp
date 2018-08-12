#include "city/Company.h"
#include "city/City.h"
#include "city/Housing.h"
#include "city/Industry.h"
#include "city/Business.h"
#include "city/Service.h"

MessageBus* Company::sMessageBus = nullptr;

void Company::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

void Company::update(float dt)
{
     // Messages
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::PERSON)
        {
            const Person::Event& event = message.getInfo<Person::Event>();
            switch (event.type)
            {
                case Person::Event::Type::LEAVE_HOUSING:
                    event.lease.setTenant(nullptr);
                    addToMarket(event.lease);
                    break;
                case Person::Event::Type::QUIT_WORK:
                    event.work.setEmployee(nullptr);
                    addToMarket(event.work);
                    break;
                default:
                    break;
            }
        }
    }
}

Company::Company(std::string name, int creationYear, Person* owner) :
    mName(std::move(name)), mCreationYear(creationYear), mOwner(owner)
{
    sMessageBus->addMailbox(mMailbox);
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

Id Company::getMailboxId() const
{
    return mMailbox.getId();
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
            if (!lease.getTenant())
                addToMarket(lease);
        }
    }
    else if (building->isIndustry())
    {
        Industry* industry = static_cast<Industry*>(building);
        for (Work& work : industry->getEmployees())
        {
            if (!work.getEmployee())
                addToMarket(work);
        }
    }
    else if (building->isBusiness())
    {
        Business* business = static_cast<Business*>(building);
        for (Work& work : business->getEmployees())
        {
            if (!work.getEmployee())
                addToMarket(work);
        }
    }
    else if (building->isService())
    {
        Service* service = static_cast<Service*>(building);
        for (Work& work : service->getEmployees())
        {
            if (!work.getEmployee())
                addToMarket(work);
        }
    }
}

float Company::getRent(Tile::Type housingType)
{
    return mRents[static_cast<int>(housingType) - static_cast<int>(Tile::Type::AFFORDABLE_HOUSING)];
}

void Company::setRent(Tile::Type housingType, float rent)
{
    mRents[static_cast<int>(housingType) - static_cast<int>(Tile::Type::AFFORDABLE_HOUSING)] = rent;
}

float Company::getSalary(Work::Qualification qualification)
{
    return mSalaries[static_cast<int>(qualification)];
}

void Company::setSalary(Work::Qualification qualification, float salary)
{
    mSalaries[static_cast<int>(qualification)] = salary;
}

void Company::addToMarket(Lease& lease)
{
    Market<Lease>* market = static_cast<Market<Lease>*>(mCity->getMarket(VMarket::Type::RENT));
    market->addItem(mMailbox.getId(), &lease, 0.0f);
}

void Company::addToMarket(Work& work)
{
    Market<Work>* market = static_cast<Market<Work>*>(mCity->getMarket(VMarket::Type::WORK));
    market->addItem(mMailbox.getId(), &work, 0.0f);
}
