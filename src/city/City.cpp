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

#include "City.h"
#include <fstream>
#include <sstream>
#include "city/Building.h"

constexpr Money City::SEED_MONEY; // Could be removed in C++17

City::Intersection::Intersection() : type(City::Intersection::Type::NONE), car(nullptr)
{

}

City::Intersection::Intersection(const Car* car) : type(City::Intersection::Type::CAR), car(car)
{

}

City::Intersection::Intersection(const Building* building) : type(City::Intersection::Type::BUILDING), building(building)
{

}

City::Event::Event()
{

}

City::Event::Event(Type type, unsigned int data) : type(type), month(data)
{

}

City::Event::Event(Type type, Person* person) : type(type), person(person)
{

}

City::Event::Event(Money minimumWage) : type(Type::NEW_MINIMUM_WAGE), minimumWage(minimumWage)
{

}

City::Event::Event(Building* building) : type(Type::BUILDING_DESTROYED), building(building)
{

}

City::City() :
    mTerrainGenerator(mRandomGenerator), mPersonGenerator(mRandomGenerator), mCompanyGenerator(mRandomGenerator),
    mCurrentTime(0.0), mTimePerMonth(20.0f), mMonth(0), mYear(0), mCityCompany("City", 0),
    mWeeklyStandardWorkingHours(0), mMinimumWage(0.0), mIncomeTax(0.0f), mCorporateTax(0.0f)
{

}

City::~City()
{
    // Unregister mailbox
    mCityMessageBus.removeMailbox(mMailbox);
}

void City::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Compute the bounds
    int margin = 3;
    sf::View view = target.getView();
    sf::Vector2f topLeft = view.getCenter() - view.getSize() * 0.5f;
    sf::Vector2i iTopLeft = toTileIndices(topLeft);
    sf::Vector2f topRight(view.getCenter().x + 0.5f * view.getSize().x, view.getCenter().y - 0.5f * view.getSize().y);
    sf::Vector2i iTopRight = toTileIndices(topRight);
    sf::Vector2f bottomLeft(view.getCenter().x - 0.5f * view.getSize().x, view.getCenter().y + 0.5f * view.getSize().y);
    sf::Vector2i iBottomLeft = toTileIndices(bottomLeft);
    sf::Vector2f bottomRight = view.getCenter() + view.getSize() * 0.5f;
    sf::Vector2i iBottomRight = toTileIndices(bottomRight);
    auto jBounds = std::minmax({iTopLeft.x, iTopRight.x, iBottomLeft.x, iBottomRight.x});
    auto iBounds = std::minmax({iTopLeft.y, iTopRight.y, iBottomLeft.y, iBottomRight.y});
    unsigned int iMin = std::max(iBounds.first - margin, 0);
    unsigned int iMax = std::min(std::max(iBounds.second + margin + 1, 0), static_cast<int>(mMap.getHeight()));
    unsigned int jMin = std::max(jBounds.first - margin, 0);
    unsigned int jMax = std::min(std::max(jBounds.second + margin + 1, 0), static_cast<int>(mMap.getWidth()));

    // Draw
    const Array2<std::unique_ptr<Tile>>& tiles = mMap.getTiles();
    for (unsigned int i = iMin; i < iMax; ++i)
    {
        for (unsigned int j = jMin; j < jMax; ++j)
        {
            target.draw(*tiles.get(i, j), states);
            for (const Car* car : mCarsByTile.get(i, j))
                target.draw(*car, states);
        }
    }
}

void City::update(float dt)
{
    // Read messages
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::CITY)
        {
            const City::Event& event = message.getInfo<City::Event>();
            switch (event.type)
            {
                case City::Event::Type::REMOVE_CITIZEN:
                    removeCitizen(event.person);
                    break;
                default:
                    break;
            }
        }
    }

    // Update the citizens
    for (Person* citizen : mCitizens)
        citizen->update(dt);

    // Update the companies
    mCityCompany.update(dt);
    for (std::unique_ptr<Company>& company : mCompanies)
        company->update(dt);

    // Update markets
    for (std::unique_ptr<MarketBase>& market : mMarkets)
        market->update();

    // Update bank
    mBank.update();

    // Update the date
    mTotalTime += dt;
    mCurrentTime += dt;
    while (mCurrentTime >= mTimePerMonth)
    {
        mCurrentTime -= mTimePerMonth;
        ++mMonth;
        onNewMonth();
    }

    // Update the map
    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
            mCarsByTile.get(i, j).clear();
    }
    for (Person* citizen : mCitizens)
    {
        if (citizen->getState() == Person::State::VISIBLE)
        {
            const Car& car = citizen->getCar();
            sf::Vector2f bottomLeft(car.getBounds().left, car.getBounds().top + car.getBounds().height);
            sf::Vector2f bottomRight(bottomLeft.x + car.getBounds().width, bottomLeft.y);
            sf::Vector2i iBottomLeft = toTileIndices(bottomLeft);
            sf::Vector2i iBottomRight = toTileIndices(bottomRight);
            sf::Vector2i indices(std::max(iBottomLeft.x, iBottomRight.x), std::max(iBottomLeft.y, iBottomRight.y));
            if (indices.y >= 0 && indices.y < static_cast<int>(mCarsByTile.getHeight()) &&
                indices.x >= 0 && indices.x < static_cast<int>(mCarsByTile.getWidth()))
                mCarsByTile.get(indices.y, indices.x).push_back(&car);
        }
    }
    // Sort the cars
    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
            std::sort(mCarsByTile.get(i, j).begin(), mCarsByTile.get(i, j).end(), [](const Car* car1, const Car* car2) { return car1->getBounds().top < car2->getBounds().top; });
    }

    // Update statistics
    updateStatistics();
}

void City::setGameMessageBus(MessageBus* messageBus)
{
    setSubjectMessageBus(messageBus);
    for (std::unique_ptr<MarketBase>& market : mMarkets)
        market->setSubjectMessageBus(messageBus);
}

Id City::getMailboxId() const
{
    return mMailbox.getId();
}

const std::string& City::getName() const
{
    return mName;
}

void City::createMap(std::string name, uint64_t seed)
{
    mName = std::move(name);

    // Generate map
    mRandomGenerator.setSeed(seed);
    mMap.fromArray(mTerrainGenerator.generate());

    // Register mailbox
    mCityMessageBus.addMailbox(mMailbox);

    // Markets
    mMarkets.emplace_back(std::make_unique<Market<Good>>(MarketBase::Type::NECESSARY_GOOD));
    mMarkets.emplace_back(std::make_unique<Market<Good>>(MarketBase::Type::NORMAL_GOOD));
    mMarkets.emplace_back(std::make_unique<Market<Good>>(MarketBase::Type::LUXURY_GOOD));
    mMarkets.emplace_back(std::make_unique<Market<Lease>>(MarketBase::Type::RENT));
    mMarkets.emplace_back(std::make_unique<Market<Work>>(MarketBase::Type::WORK));

    // Economy
    mWorldAccount = mBank.createWorldAccount();

    // Set up
    setUp(false);
}

void City::bulldoze(Tile::Type type)
{
    // Bulldoze the map
    std::vector<Id> buildingsToRemove;
    mMap.bulldoze(type, mCityCompany, mBuildings, buildingsToRemove);
    // Notify the changes
    for (Id id : buildingsToRemove)
        notify(Message::create(MessageType::CITY, Event(getBuilding(id))));
    // Destroy the buildings
    for (Id id : buildingsToRemove)
        mBuildings.erase(id);
}

City::Intersection City::intersect(const sf::Vector2f& position)
{
    Intersection intersection;

    const Array2<std::unique_ptr<Tile>>& tiles = mMap.getTiles();
    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
        {
            const Tile* tile = tiles.get(i, j).get();
            if (tile->intersect(position))
            {
                if (tile->isBuilding())
                    intersection = Intersection(static_cast<const Building*>(tile));
                else
                    intersection = Intersection();
            }
            for (const Car* car : mCarsByTile.get(i, j))
            {
                if (car->intersect(position))
                    intersection = Intersection(car);
            }
        }
    }
    return intersection;
}

Map& City::getMap()
{
    return mMap;
}

const Map& City::getMap() const
{
    return mMap;
}

unsigned int City::getMonth() const
{
    return mMonth;
}

unsigned int City::getYear() const
{
    return mYear;
}

std::string City::getFormattedMonth() const
{
    switch (mMonth)
    {
        case 0:
            return "January";
        case 1:
            return "February";
        case 2:
            return "March";
        case 3:
            return "April";
        case 4:
            return "May";
        case 5:
            return "June";
        case 6:
            return "July";
        case 7:
            return "August";
        case 8:
            return "September";
        case 9:
            return "October";
        case 10:
            return "November";
        case 11:
            return "December";
        default:
            return "";
    }
}

const Bank& City::getBank() const
{
    return mBank;
}

MarketBase* City::getMarket(MarketBase::Type type)
{
    return mMarkets[static_cast<int>(type)].get();
}

const MarketBase* City::getMarket(MarketBase::Type type) const
{
    return mMarkets[static_cast<int>(type)].get();
}

Company& City::getCompany()
{
    return mCityCompany;
}

Money City::getFunds() const
{
    return mCityCompany.getAccountBalance();
}

void City::decreaseFunds(Money amount)
{
    mBank.transferMoney(mCityCompany.getAccount(), mWorldAccount, amount);
}

unsigned int City::getWeeklyStandardWorkingHours() const
{
    return mWeeklyStandardWorkingHours;
}

void City::setWeeklyStandardWorkingHours(unsigned int weeklyStandardWorkingHours)
{
    mWeeklyStandardWorkingHours = weeklyStandardWorkingHours;
}

Money City::getMinimumWage() const
{
    return mMinimumWage;
}

void City::setMinimumWage(Money minimumWage)
{
    mMinimumWage = minimumWage;
    // Send messages
    mCityMessageBus.send(Message::create(mCityCompany.getMailboxId(), MessageType::CITY, Event(mMinimumWage)));
    for (std::unique_ptr<Company>& company : mCompanies)
        mCityMessageBus.send(Message::create(company->getMailboxId(), MessageType::CITY, Event(mMinimumWage)));
}

double City::getIncomeTax() const
{
    return mIncomeTax;
}

void City::setIncomeTax(double incomeTax)
{
    mIncomeTax = incomeTax;
}

double City::getCorporateTax() const
{
    return mCorporateTax;
}

void City::setCorporateTax(double corporateTax)
{
    mCorporateTax = corporateTax;
}

void City::eject(Person* person)
{
    std::size_t i = std::find(mImmigrants.begin(), mImmigrants.end(), person) - mImmigrants.begin();
    mImmigrants.erase(mImmigrants.begin() + i);
    mTimeBeforeLeaving.erase(mTimeBeforeLeaving.begin() + i);
    mPersons.erase(person->getId());
    // Notify
    notify(Message::create(MessageType::CITY, Event(Event::Type::IMMIGRANT_EJECTED, person)));
}

void City::welcome(Person* person)
{
    std::size_t i = std::find(mImmigrants.begin(), mImmigrants.end(), person) - mImmigrants.begin();
    mImmigrants.erase(mImmigrants.begin() + i);
    mTimeBeforeLeaving.erase(mTimeBeforeLeaving.begin() + i);
    mCitizens.push_back(person);
    person->setCity(this, &mCityMessageBus);
    // Notify
    notify(Message::create(MessageType::CITY, Event(Event::Type::NEW_CITIZEN, person)));
}

unsigned int City::getPopulation() const
{
    return mCitizens.size();
}

Person* City::getPerson(Id id)
{
    return mPersons.get(id).get();
}

const std::vector<Person*>& City::getCitizens() const
{
    return mCitizens;
}

const std::vector<Person*>& City::getImmigrants() const
{
    return mImmigrants;
}

Building* City::getBuilding(Id id)
{
    return mBuildings.get(id);
}

float City::getAverageHappiness() const
{
    return mHappiness;
}

float City::getAttractiveness() const
{
    return mAttractiveness;
}

sf::Vector2i City::toTileIndices(const sf::Vector2f& position) const
{
    int x = position.y / Tile::HEIGHT + 0.5f * (position.x / Tile::HEIGHT - mMap.getWidth() - 1);
    int y = position.y / Tile::HEIGHT - 0.5f * (position.x / Tile::HEIGHT - mMap.getWidth() - 1);
    return sf::Vector2i(x, y);
}

float City::getHumanTime() const
{
    return mTotalTime;
}

float City::getTimePerMonth() const
{
    return mTimePerMonth;
}

float City::computeNbHoursInAmonth(float nbHoursInAWeek) const
{
    return 30.0f / 7.0f * nbHoursInAWeek;
}

float City::toHumanTime(float cityTime) const
{
    return cityTime / NB_HOURS_PER_MONTH * mTimePerMonth;
}

float City::toCityTime(float humanTime) const
{
    return humanTime / mTimePerMonth * NB_HOURS_PER_MONTH;
}

void City::updateImmigrants()
{
    // Update immigrants
    for (int i = mImmigrants.size() - 1; i >= 0; --i)
    {
        --mTimeBeforeLeaving[i];
        if (mTimeBeforeLeaving[i] == 0)
            eject(mImmigrants[i]); // Could be optimized
    }

    // Add immigrants
    std::poisson_distribution<unsigned int> distribution(mAttractiveness * MAX_NB_IMMIGRANTS_PER_MONTH);
    unsigned int nbNewImmigrants = distribution(mRandomGenerator);
    for (unsigned int i = 0; i < nbNewImmigrants; ++i)
        generateImmigrant();
}

void City::generateImmigrant()
{
    std::unique_ptr<Person> person = mPersonGenerator.generate(getYear());
    mImmigrants.push_back(person.get());
    mTimeBeforeLeaving.push_back(MAX_NB_MONTHS_WAITING);
    Id id = mPersons.add(std::move(person));
    mImmigrants.back()->setId(id);
    // Notify
    notify(Message::create(MessageType::CITY, Event(Event::Type::NEW_IMMIGRANT, mImmigrants.back())));
}

void City::removeCitizen(Person* person)
{
    mCitizens.erase(std::find(mCitizens.begin(), mCitizens.end(), person));
    mPersons.erase(person->getId());
    // Notify
    notify(Message::create(MessageType::CITY, Event(Event::Type::CITIZEN_LEFT, person)));
}

void City::updateStatistics()
{
    computeHappiness();
    computeAttractiveness();
}

void City::computeHappiness()
{
    if (mCitizens.empty())
        mHappiness = 1.0f;
    else
    {
        float sumAverage = std::accumulate(mCitizens.begin(), mCitizens.end(), 0.0f, [](float lhs, Person* rhs){ return lhs + rhs->getNeed(Person::Need::HAPPINESS); });
        mHappiness = sumAverage / mCitizens.size();
    }
}

void City::computeAttractiveness()
{
    mAttractiveness = 1.0f;
    // Ease to obtain a home
    float nbHomesAvailable = static_cast<const Market<Lease>*>(getMarket(MarketBase::Type::RENT))->getItems().size();
    mAttractiveness *= 1 - 1 / (1.0f + nbHomesAvailable);
    // Ease to obtain a job
    float nbWorksAvailable = static_cast<const Market<Work>*>(getMarket(MarketBase::Type::WORK))->getItems().size();
    mAttractiveness *= 1 - 1 / (1.0f + nbWorksAvailable);
    // Happiness
    mAttractiveness *= getAverageHappiness();
}

void City::onNewMonth()
{
    // Update year
    if (mMonth >= 12)
    {
        mMonth = 0;
        ++mYear;
        onNewYear();
    }

    // Update immigrants
    updateImmigrants();

    // Update markets
    for (std::unique_ptr<MarketBase>& market : mMarkets)
        market->sellItems();

    // Collect taxes
    mBank.collectTaxes(mCityCompany.getAccount(), mIncomeTax, mCorporateTax);

    // Send messages
    notify(Message::create(MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
    for (Person* citizen : mCitizens)
        mCityMessageBus.send(Message::create(citizen->getMailboxId(), MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
    mCityMessageBus.send(Message::create(mCityCompany.getMailboxId(), MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
    for (std::unique_ptr<Company>& company : mCompanies)
        mCityMessageBus.send(Message::create(company->getMailboxId(), MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
}

void City::onNewYear()
{
    // Send messages
    notify(Message::create(MessageType::CITY, Event(Event::Type::NEW_YEAR, mYear)));
}

void City::setUp(bool loading)
{
    // Generators
    mPersonGenerator.setUp();
    mCompanyGenerator.setUp();

    // Bank
    mBank.setMessageBus(&mCityMessageBus, loading);

    // Markets
    for (std::unique_ptr<MarketBase>& market : mMarkets)
        market->setMessageBus(&mCityMessageBus, loading);

    // Citizens
    for (Person* citizen : mCitizens)
        citizen->setCity(this, &mCityMessageBus, loading);

    // Companies
    mCityCompany.setCity(this, &mCityMessageBus, loading);
    for (std::unique_ptr<Company>& company : mCompanies)
        company->setCity(this, &mCityMessageBus, loading);

    // Rendering
    mCarsByTile.reshape(mMap.getHeight(), mMap.getWidth());

    // Init the funds
    if (!loading)
    {
        mBank.update();
        mCityCompany.update(0.0f);
        mBank.transferMoney(mWorldAccount, mCityCompany.getAccount(), SEED_MONEY);
    }
}
