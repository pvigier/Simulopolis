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

std::vector<Work>* Company::getEmployees(Building* building)
{
    if (building->isIndustry())
        return &static_cast<Industry*>(building)->getEmployees();
    else if (building->isBusiness())
        return &static_cast<Business*>(building)->getEmployees();
    else if (building->isService())
        return &static_cast<Service*>(building)->getEmployees();
    else
        return nullptr;
}

const std::vector<Work>* Company::getEmployees(const Building* building)
{
    if (building->isIndustry())
        return &static_cast<const Industry*>(building)->getEmployees();
    else if (building->isBusiness())
        return &static_cast<const Business*>(building)->getEmployees();
    else if (building->isService())
        return &static_cast<const Service*>(building)->getEmployees();
    else
        return nullptr;
}

Company::Company(std::string name, int creationYear, Person* owner) :
    mName(std::move(name)), mCreationYear(creationYear), mOwner(owner), mFunds(0.0)
{
    mRents.fill(Money(0.0));
    mSalaries.fill(Money(0.0));
    sMessageBus->addMailbox(mMailbox);
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

const std::string& Company::getName() const
{
    return mName;
}

City* Company::getCity()
{
    return mCity;
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
    else
    {
        for (Work& work : *getEmployees(building))
        {
            if (!work.getEmployee())
                addToMarket(work);
        }
    }
}

Money Company::getRent(Tile::Type housingType)
{
    return mRents[static_cast<int>(housingType) - static_cast<int>(Tile::Type::AFFORDABLE_HOUSING)];
}

void Company::setRent(Tile::Type housingType, Money rent)
{
    mRents[static_cast<int>(housingType) - static_cast<int>(Tile::Type::AFFORDABLE_HOUSING)] = rent;
    for (Building* building : mBuildings)
    {
        if (building->getType() == housingType)
        {
            for (Lease& lease : static_cast<Housing*>(building)->getLeases())
                lease.setRent(rent);
        }
    }
}

Money Company::getSalary(Work::Qualification qualification)
{
    return mSalaries[static_cast<int>(qualification)];
}

void Company::setSalary(Work::Qualification qualification, Money salary)
{
    mSalaries[static_cast<int>(qualification)] = salary;
    for (Building* building : mBuildings)
    {
        if (!building->isHousing())
        {
            for (Work& work : *getEmployees(building))
            {
                if (work.getQualification() == qualification)
                    work.setSalary(salary);
            }
        }
    }
}

void Company::addToMarket(Lease& lease)
{
    Market<Lease>* market = static_cast<Market<Lease>*>(mCity->getMarket(VMarket::Type::RENT));
    market->addItem(mMailbox.getId(), &lease, lease.getRent());
}

void Company::addToMarket(Work& work)
{
    Market<Work>* market = static_cast<Market<Work>*>(mCity->getMarket(VMarket::Type::WORK));
    market->addItem(mMailbox.getId(), &work, work.getSalary());
}
