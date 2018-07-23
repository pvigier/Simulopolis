#include "City.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Building.h"

City::Intersection::Intersection() : type(City::Intersection::Type::NONE), car(nullptr)
{

}

City::Intersection::Intersection(const Car* car) : type(City::Intersection::Type::CAR), car(car)
{

}

City::Intersection::Intersection(const Building* building) : type(City::Intersection::Type::BUILDING), building(building)
{

}

City::City() : mCurrentTime(0.0), mTimePerMonth(120.0f), mMonth(0), mPopulation(0), mUnemployed(0), mFunds(0)
{
    // Generators
    mPersonGenerator.setUp();
    mCompanyGenerator.setUp();

    // Markets
    mMarkets.emplace_back(new Market<>(VMarket::Type::NECESSARY_GOOD));
    mMarkets.emplace_back(new Market<>(VMarket::Type::NORMAL_GOOD));
    mMarkets.emplace_back(new Market<>(VMarket::Type::LUXURY_GOOD));
    mMarkets.emplace_back(new Market<Housing>(VMarket::Type::AFFORDABLE_HOUSING_RENT));
    mMarkets.emplace_back(new Market<Housing>(VMarket::Type::APARTMENT_BUILDING_RENT));
    mMarkets.emplace_back(new Market<Housing>(VMarket::Type::VILLA_RENT));
    mMarkets.emplace_back(new Market<Work>(VMarket::Type::NON_QUALIFIED_JOB));
    mMarkets.emplace_back(new Market<Work>(VMarket::Type::QUALIFIED_JOB));
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
                    mUnemployed = std::stod(value);
                else if(key == "population")
                    mPopulation = std::stod(value);
                else if(key == "funds")
                    mFunds = std::stod(value);
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

    mPersons.push_back(mPersonGenerator.generate(getYear()));
    Path path = mMap.getPath(sf::Vector2i(0, 0), sf::Vector2i(2, 0));
    mPersons.back()->getCar().getKinematic().setPosition(path.getCurrentPoint());
    mPersons.back()->getCar().getSteering().setPath(path);
    mPersons.back()->setState(Person::State::MOVING);

    mPersons.push_back(mPersonGenerator.generate(getYear()));
    Path otherPath = mMap.getPath(sf::Vector2i(2, 0), sf::Vector2i(0, 0));
    mPersons.back()->getCar().getKinematic().setPosition(otherPath.getCurrentPoint());
    mPersons.back()->getCar().getSteering().setPath(otherPath);
    mPersons.back()->setState(Person::State::MOVING);
}

void City::save(const std::string& name)
{
    std::ofstream outputFile(name + "_cfg.dat", std::ios::out);

    outputFile << "width=" << mMap.getWidth() << std::endl;
    outputFile << "height=" << mMap.getHeight() << std::endl;
    outputFile << "month=" << mMonth << std::endl;
    outputFile << "population=" << mPopulation << std::endl;
    outputFile << "unemployed=" << mUnemployed << std::endl;
    outputFile << "funds=" << mFunds << std::endl;

    outputFile.close();

    mMap.save(name + "_map.dat");
}

void City::createMap(const Array2<Tile::Type>& tiles)
{
    mMap.fromArray(tiles);
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
    unsigned int iMax = std::min(iBounds.second + margin + 1, static_cast<int>(mMap.getHeight()) - 1);
    unsigned int jMin = std::max(jBounds.first - margin, 0);
    unsigned int jMax = std::min(jBounds.second + margin + 1, static_cast<int>(mMap.getWidth()) - 1);

    // Draw
    const Array2<std::unique_ptr<Tile>>& tiles = mMap.getTiles();
    for (unsigned int i = iMin; i < iMax; ++i)
    {
        for (unsigned int j = jMin; j < jMax; ++j)
        {
            target.draw(*tiles.get(i, j));
            for (const Car* car : mCarsByTile.get(i, j))
                target.draw(*car);
        }
    }
}

void City::update(float dt)
{
    for (std::unique_ptr<Person>& person : mPersons)
        person->update(dt);

    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
            mCarsByTile.get(i, j).clear();
    }
    for (const std::unique_ptr<Person>& person : mPersons)
    {
        if (person->getState() == Person::State::MOVING)
        {
            const Car& car = person->getCar();
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
}

void City::bulldoze(Tile::Type type)
{
    mMap.bulldoze(type);
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
    return mMonth / 12;
}

unsigned int City::getPopulation() const
{
    return mPopulation;
}

unsigned int City::getUnemployed() const
{
    return mUnemployed;
}

unsigned int City::getFunds() const
{
    return mFunds;
}

void City::decreaseFunds(unsigned int amount)
{
    mFunds -= amount;
}

VMarket* City::getMarket(VMarket::Type type)
{
    return mMarkets[static_cast<int>(type)].get();
}

sf::Vector2i City::toTileIndices(const sf::Vector2f& position) const
{
    int x = position.y / Tile::SIZE + 0.5f * (position.x / Tile::SIZE - mMap.getWidth() - 1);
    int y = position.y / Tile::SIZE - 0.5f * (position.x / Tile::SIZE - mMap.getWidth() - 1);
    return sf::Vector2i(x, y);
}

float City::toHumanTime(float cityTime) const
{
    return cityTime / (30.0f * 24.0f) * mTimePerMonth;
}
