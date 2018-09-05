#include "City.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "city/Building.h"

City::Intersection::Intersection() : type(City::Intersection::Type::NONE), car(nullptr)
{

}

City::Intersection::Intersection(const Car* car) : type(City::Intersection::Type::CAR), car(car)
{

}

City::Intersection::Intersection(const Building* building) : type(City::Intersection::Type::BUILDING), building(building)
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
    mCurrentTime(0.0), mTimePerMonth(10.0f), mMonth(0), mYear(0), mCityCompany("City", 0),
    mWeeklyStandardWorkingHours(0), mMinimumWage(0.0), mIncomeTax(0.0f), mCorporateTax(0.0f)
{
    // Generators
    mPersonGenerator.setUp();
    mCompanyGenerator.setUp();

    // Markets
    mMarkets.emplace_back(new Market<const Building>(VMarket::Type::NECESSARY_GOOD));
    mMarkets.emplace_back(new Market<const Building>(VMarket::Type::NORMAL_GOOD));
    mMarkets.emplace_back(new Market<const Building>(VMarket::Type::LUXURY_GOOD));
    mMarkets.emplace_back(new Market<Lease>(VMarket::Type::RENT));
    mMarkets.emplace_back(new Market<Work>(VMarket::Type::WORK));

    // Economy
    mWorldAccount = mBank.createAccount();

    // Company
    mCityCompany.setCity(this);
}

void City::load(const std::string& name)
{
    int width = 0;
    int height = 0;

    std::ifstream inputFile(name + "_cfg.dat", std::ios::in);
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream lineStream(line);
        std::string key;
        if(std::getline(lineStream, key, '='))
        {
            std::string value;
            if(std::getline(lineStream, value))
            {
                if(key == "width")
                    width = std::stoi(value);
                else if(key == "height")
                    height = std::stoi(value);
                else if(key == "month")
                    mMonth = std::stoi(value);
                else if(key == "unemployed")
                    break;
                else if(key == "funds")
                    break;
            }
            else
                std::cerr << "Error, no value for key " << key << std::endl;
        }
    }

    inputFile.close();

    mMap.load(name + "_map.dat", width, height);
    mCarsByTile.reshape(mMap.getHeight(), mMap.getWidth());

    // Tests
    mMap.select(sf::Vector2i(0, 0), sf::Vector2i(0, 10), all<Tile::Category>);
    mMap.select(sf::Vector2i(1, 10), sf::Vector2i(10, 10), all<Tile::Category>);
    mMap.select(sf::Vector2i(10, 9), sf::Vector2i(10, 0), all<Tile::Category>);
    mMap.select(sf::Vector2i(9, 0), sf::Vector2i(2, 0), all<Tile::Category>);
    bulldoze(Tile::Type::ROAD_GRASS);

    /*generateImmigrant();
    welcome(mImmigrants.back());
    Path path = mMap.getPath(sf::Vector2i(0, 0), sf::Vector2i(2, 0));
    mCitizens.back()->getCar().getKinematic().setPosition(path.getCurrentPoint());
    mCitizens.back()->getCar().getSteering().setPath(path);
    mCitizens.back()->setState(Person::State::MOVING);

    generateImmigrant();
    welcome(mImmigrants.back());
    Path otherPath = mMap.getPath(sf::Vector2i(2, 0), sf::Vector2i(0, 0));
    mCitizens.back()->getCar().getKinematic().setPosition(otherPath.getCurrentPoint());
    mCitizens.back()->getCar().getSteering().setPath(otherPath);
    mCitizens.back()->setState(Person::State::MOVING);*/

    // tmp
    for (int i = 0; i < 100; ++i)
        generateImmigrant();
    for (std::unique_ptr<Person>& person : mPersons.getObjects())
        welcome(person.get());
}

void City::save(const std::string& name)
{
    std::ofstream outputFile(name + "_cfg.dat", std::ios::out);

    outputFile << "width=" << mMap.getWidth() << std::endl;
    outputFile << "height=" << mMap.getHeight() << std::endl;
    outputFile << "month=" << mMonth << std::endl;
    outputFile << "population=" << 0 << std::endl;
    outputFile << "unemployed=" << 0 << std::endl;
    outputFile << "funds=" << 0 << std::endl;

    outputFile.close();

    mMap.save(name + "_map.dat");
}

void City::createMap(uint64_t seed)
{
    mRandomGenerator.setSeed(seed);
    mMap.fromArray(mTerrainGenerator.generate());
    mCarsByTile.reshape(mMap.getHeight(), mMap.getWidth());
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
    // Update the citizens
    for (Person* citizen : mCitizens)
        citizen->update(dt);

    // Update the companies
    mCityCompany.update(dt);
    for (std::unique_ptr<Company>& company : mCompanies)
        company->update(dt);

    // Update markets
    for (std::unique_ptr<VMarket>& market : mMarkets)
        market->update();

    // Update bank
    mBank.update();

    // Update the date
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
        if (citizen->getState() == Person::State::MOVING)
        {
            const Car& car = citizen->getCar();
            sf::Vector2f bottomLeft(car.getBounds().left, car.getBounds().top + car.getBounds().height);
            sf::Vector2f bottomRight(bottomLeft.x + car.getBounds().width, bottomLeft.y);
            sf::Vector2i iBottomLeft = toTileIndices(bottomLeft);
            sf::Vector2i iBottomRight = toTileIndices(bottomRight);
            sf::Vector2i indices(std::max(iBottomLeft.x, iBottomRight.x), std::max(iBottomLeft.y, iBottomRight.y));
            mCarsByTile.get(indices.y, indices.x).push_back(&car);
        }
    }
    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
            std::sort(mCarsByTile.get(i, j).begin(), mCarsByTile.get(i, j).end(), [](const Car* car1, const Car* car2) { return car1->getBounds().top < car2->getBounds().top; });
    }

    // Update statistics
    updateStatistics();
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

VMarket* City::getMarket(VMarket::Type type)
{
    return mMarkets[static_cast<int>(type)].get();
}

const VMarket* City::getMarket(VMarket::Type type) const
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
    sMessageBus->send(Message::create(mCityCompany.getMailboxId(), MessageType::CITY, Event(mMinimumWage)));
    for (std::unique_ptr<Company>& company : mCompanies)
        sMessageBus->send(Message::create(company->getMailboxId(), MessageType::CITY, Event(mMinimumWage)));
}

float City::getIncomeTax() const
{
    return mIncomeTax;
}

void City::setIncomeTax(float incomeTax)
{
    mIncomeTax = incomeTax;
}

float City::getCorporateTax() const
{
    return mCorporateTax;
}

void City::setCorporateTax(float corporateTax)
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
    person->setCity(this);
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
    int x = position.y / Tile::SIZE + 0.5f * (position.x / Tile::SIZE - mMap.getWidth() - 1);
    int y = position.y / Tile::SIZE - 0.5f * (position.x / Tile::SIZE - mMap.getWidth() - 1);
    return sf::Vector2i(x, y);
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
    // Should send a message
}

void City::updateStatistics()
{
    computeHappiness();
    computeAttractiveness();
}

void City::computeHappiness()
{
    if (mCitizens.empty())
        mHappiness = 100.0f;
    else
    {
        float sumAverage = std::accumulate(mCitizens.begin(), mCitizens.end(), 0.0f, [](float lhs, Person* rhs){ return lhs + rhs->getHappiness(); });
        mHappiness = sumAverage / mCitizens.size();
    }
}

void City::computeAttractiveness()
{
    mAttractiveness = 1.0f;
    // Ease to obtain a home
    float nbHomesAvailable = static_cast<const Market<Lease>*>(getMarket(VMarket::Type::RENT))->getItems().size();
    mAttractiveness *= 1 - 1 / (1.0f + nbHomesAvailable);
    // Ease to obtain a job
    float nbWorksAvailable = static_cast<const Market<Work>*>(getMarket(VMarket::Type::WORK))->getItems().size();
    mAttractiveness *= 1 -  1 / (1.0f + nbWorksAvailable);
    // Happiness
    mAttractiveness *= getAverageHappiness() / 100.0f;
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
    for (std::unique_ptr<VMarket>& market : mMarkets)
        market->sellItems();

    // Send messages
    notify(Message::create(MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
    for (Person* citizen : mCitizens)
        sMessageBus->send(Message::create(citizen->getMailboxId(), MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
    sMessageBus->send(Message::create(mCityCompany.getMailboxId(), MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
    for (std::unique_ptr<Company>& company : mCompanies)
        sMessageBus->send(Message::create(company->getMailboxId(), MessageType::CITY, Event(Event::Type::NEW_MONTH, mMonth)));
}

void City::onNewYear()
{
    // Send messages
    notify(Message::create(MessageType::CITY, Event(Event::Type::NEW_YEAR, mYear)));
}
