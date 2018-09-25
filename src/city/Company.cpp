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

#include "city/Company.h"
#include "city/City.h"
#include "city/Housing.h"
#include "city/Industry.h"
#include "city/Business.h"
#include "city/Service.h"

std::vector<std::unique_ptr<Work>>* Company::getEmployees(Building* building)
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

const std::vector<std::unique_ptr<Work>>* Company::getEmployees(const Building* building)
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

Company::Company(std::string name, int creationYear, Person* owner, Money funds) :
    mName(std::move(name)), mCreationYear(creationYear),
    mCity(nullptr), mMessageBus(nullptr), mOwner(owner), mFunds(funds), mAccount(UNDEFINED)
{
    mRents.fill(Money(0.0));
    mSalaries.fill(Money(0.0));
    mWholesaleMargins.fill(0.0);
    mRetailMargins.fill(0.0);
}

Company::~Company()
{
    // Close bank account
    if (mAccount != UNDEFINED)
        mMessageBus->send(Message::create(mMailbox.getId(), mCity->getBank().getMailboxId(), MessageType::BANK, mCity->getBank().createCloseAccountEvent(mAccount)));
    // Unregister mailbox
    if (mMailbox.getId() != UNDEFINED)
        mMessageBus->removeMailbox(mMailbox);
}

void Company::update(float /*dt*/)
{
     // Messages
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::CITY)
        {
            const City::Event& event = message.getInfo<City::Event>();
            switch (event.type)
            {
                case City::Event::Type::NEW_MONTH:
                    onNewMonth();
                    break;
                case City::Event::Type::NEW_MINIMUM_WAGE:
                    onNewMinimumWage(event.minimumWage);
                    break;
                default:
                    break;
            }
        }
        else if (message.type == MessageType::BANK)
        {
            const Bank::Event& event = message.getInfo<Bank::Event>();
            switch (event.type)
            {
                case Bank::Event::Type::ACCOUNT_CREATED:
                    mAccount = event.account;
                    break;
                default:
                    break;
            }
        }
        else if (message.type == MessageType::PERSON)
        {
            const Person::Event& event = message.getInfo<Person::Event>();
            switch (event.type)
            {
                case Person::Event::Type::LEAVE_HOUSING:
                    event.lease->setTenant(nullptr);
                    addToMarket(event.lease);
                    break;
                case Person::Event::Type::QUIT_WORK:
                    event.work->setEmployee(nullptr);
                    addToMarket(event.work);
                    break;
                default:
                    break;
            }
        }
    }

    // Update buildings
    for (Building* building : mBuildings)
        building->update();
}

const std::string& Company::getName() const
{
    return mName;
}

const City* Company::getCity()
{
    return mCity;
}

void Company::setCity(const City* city, MessageBus* messageBus, bool alreadyAdded)
{
    mCity = city;
    mMessageBus = messageBus;
    if (!alreadyAdded)
    {
        mMessageBus->addMailbox(mMailbox);
        // Create bank account
        mMessageBus->send(Message::create(mMailbox.getId(), mCity->getBank().getMailboxId(), MessageType::BANK, mCity->getBank().createCreateAccountEvent(Bank::Account::Type::COMPANY, mFunds)));
    }
}

MessageBus* Company::getMessageBus()
{
    return mMessageBus;
}


const Person* Company::getOwner() const
{
    return mOwner;
}

void Company::setOwner(const Person* owner)
{
    mOwner = owner;
}

Id Company::getMailboxId() const
{
    return mMailbox.getId();
}

Id Company::getAccount() const
{
    return mAccount;
}

Money Company::getAccountBalance() const
{
    if (mAccount != UNDEFINED)
        return mCity->getBank().getBalance(mAccount);
    return Money(0.0);
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
        for (std::unique_ptr<Lease>& lease : housing->getLeases())
        {
            if (!lease->getTenant())
                addToMarket(lease.get());
        }
    }
    else
    {
        for (std::unique_ptr<Work>& work : *getEmployees(building))
        {
            if (!work->getEmployee())
                addToMarket(work.get());
        }
    }
}

void Company::removeBuilding(Building* building)
{
    // Alert the tenants
    if (building->isHousing())
    {
        Housing* housing = static_cast<Housing*>(building);
        for (std::unique_ptr<Lease>& lease : housing->getLeases())
        {
            if (lease->getTenant())
                mMessageBus->send(Message::create(lease->getTenant()->getMailboxId(), MessageType::PERSON, Person::Event{Person::Event::Type::EXPELLED}));
        }
    }
    // Alert the employees
    else
    {
        for (std::unique_ptr<Work>& work : *getEmployees(building))
        {
            if (work->getEmployee())
            {
                // Pay the employee if necessary
                if (work->hasWorkedThisMonth())
                    mMessageBus->send(Message::create(mMailbox.getId(), mCity->getBank().getMailboxId(), MessageType::BANK, mCity->getBank().createTransferMoneyEvent(mAccount, work->getEmployee()->getAccount(), work->getSalary())));
                mMessageBus->send(Message::create(work->getEmployee()->getMailboxId(), MessageType::PERSON, Person::Event{Person::Event::Type::FIRED}));
            }
        }
    }
    // Tear down the building
    building->tearDown();
    // Remove from the list of buildings
    mBuildings.erase(std::find(mBuildings.begin(), mBuildings.end(), building));
}

Money Company::getRent(Tile::Type housingType) const
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
            for (std::unique_ptr<Lease>& lease : static_cast<Housing*>(building)->getLeases())
                lease->setRent(rent);
        }
    }
}

Money Company::getSalary(Work::Qualification qualification) const
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
            for (std::unique_ptr<Work>& work : *getEmployees(building))
            {
                if (work->getQualification() == qualification)
                    work->setSalary(salary);
            }
        }
    }
}

double Company::getWholesaleMargin(Good::Type goodType) const
{
    return mWholesaleMargins[static_cast<int>(goodType)];
}

void Company::setWholesaleMargin(Good::Type goodType, double margin)
{
    mWholesaleMargins[static_cast<int>(goodType)] = margin;
}

double Company::getRetailMargin(Good::Type goodType) const
{
     return mRetailMargins[static_cast<int>(goodType)];
}

void Company::setRetailMargin(Good::Type goodType, double margin)
{
    mRetailMargins[static_cast<int>(goodType)] = margin;
}

void Company::addToMarket(Lease* lease)
{
    const Market<Lease>* market = static_cast<const Market<Lease>*>(mCity->getMarket(MarketBase::Type::RENT));
    mMessageBus->send(Message::create(lease->getHousing()->getMailboxId(), market->getMailboxId(), MessageType::MARKET, market->createAddItemEvent(mAccount, lease, lease->getRent())));
}

void Company::addToMarket(Work* work)
{
    const Market<Work>* market = static_cast<const Market<Work>*>(mCity->getMarket(MarketBase::Type::WORK));
    mMessageBus->send(Message::create(work->getWorkplace()->getMailboxId(), market->getMailboxId(), MessageType::MARKET, market->createAddItemEvent(mAccount, work, work->getSalary())));
}

void Company::onNewMonth()
{
    // Update buildings
    for (Building* building : mBuildings)
    {
        if (building->isIndustry())
            static_cast<Industry*>(building)->onNewMonth();
        else if (building->isBusiness())
            static_cast<Business*>(building)->onNewMonth();
    }

    // Pay salaries
    for (Building* building : mBuildings)
    {
        if (!building->isHousing())
        {
            for (std::unique_ptr<Work>& work : *getEmployees(building))
            {
                if (work->hasWorkedThisMonth())
                    mMessageBus->send(Message::create(mMailbox.getId(), mCity->getBank().getMailboxId(), MessageType::BANK, mCity->getBank().createTransferMoneyEvent(mAccount, work->getEmployee()->getAccount(), work->getSalary())));
                work->setWorkedThisMonth(false);
            }
        }
    }
}

void Company::onNewMinimumWage(Money minimumWage)
{
    for (Building* building : mBuildings)
    {
        if (!building->isHousing())
        {
            for (std::unique_ptr<Work>& work : *getEmployees(building))
                work->setSalary(std::max(work->getSalary(), minimumWage));
        }
    }
}
