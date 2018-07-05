#include "Map.h"
#include <fstream>
#include "resource/TextureManager.h"
#include "city/Road.h"
#include "city/Building.h"
#include "city/CallForBids.h"
#include "city/Housing.h"

std::vector<std::unique_ptr<Tile>> Map::sTileAtlas;

Map::Map() : mWidth(0), mHeight(0), mNbSelected(0), mNetwork(mWidth, mHeight)
{

}

void Map::loadTiles(const TextureManager& textureManager)
{
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile("grass", Tile::Type::VOID, Tile::Category::GROUND)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile("grass", Tile::Type::GRASS, Tile::Category::GROUND)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile("forest", Tile::Type::FOREST, Tile::Category::OBSTACLE)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile("water", Tile::Type::WATER, Tile::Category::WATER)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile("dirt", Tile::Type::DIRT, Tile::Category::OBSTACLE)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new CallForBids("grass", Tile::Type::CFB_HOUSING)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Housing("housing", Tile::Type::AFFORDABLE_HOUSING, 3, 9, 5)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Housing("housing", Tile::Type::APARTMENT_BUILDING, 2, 3, 10)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Housing("housing", Tile::Type::VILLA, 1, 1, 20)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new CallForBids("grass", Tile::Type::CFB_INDUSTRY)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("industry", Tile::Type::FARM, 1)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("industry", Tile::Type::FACTORY, 3)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("industry", Tile::Type::WORKSHOP, 2)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new CallForBids("grass", Tile::Type::CFB_BUSINESS)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("business", Tile::Type::MARKET, 1)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("business", Tile::Type::MALL, 3)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("business", Tile::Type::BOUTIQUE, 2)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new CallForBids("grass", Tile::Type::CFB_HOSPITAL)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("hospital", Tile::Type::HOSPITAL, 2)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new CallForBids("grass", Tile::Type::CFB_POLICE_STATION)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("police", Tile::Type::POLICE_STATION, 2)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new CallForBids("grass", Tile::Type::CFB_SCHOOL)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building("school", Tile::Type::SCHOOL, 2)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Road("road", Tile::Type::ROAD_GRASS)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Road("road", Tile::Type::ROAD_SIDEWALK)));
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Road("road", Tile::Type::ROAD_WATER)));
}

const Map::TileAtlas& Map::getTileAtlas()
{
    return sTileAtlas;
}

void Map::load(const std::string& filename, unsigned int width, unsigned int height)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);

    mWidth = width;
    mHeight = height;
    mTiles.reshape(mHeight, mWidth);

    for (unsigned int i = 0; i < mHeight; ++i)
    {
        for (unsigned int j = 0; j < mWidth; ++j)
        {
            Tile::Type type;
            inputFile.read((char*)&type, sizeof(type));
            mTiles.set(i, j, createTile(type));
            mTiles.get(i, j)->setPosition(computePosition(i, j));
            char tmp[4];
            inputFile.read(tmp, sizeof(unsigned int));
            inputFile.read(tmp, sizeof(unsigned int));
            inputFile.read(tmp, sizeof(double));
            inputFile.read(tmp, sizeof(float));
        }
    }

    inputFile.close();

    for (unsigned int i = 0; i < mHeight; ++i)
    {
        for (unsigned int j = 0; j < mWidth; ++j)
            updateTile(i, j);
    }

    mNetwork.reshape(mWidth, mHeight);
}

void Map::save(const std::string& filename)
{
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::binary);

    /*for (const std::unique_ptr<Tile>& tile : mTiles)
    {
        Tile::Type type = tile->getType();
        outputFile.write((char*)&type, sizeof(Tile::Type));
        outputFile.write((char*)&tile.getVariant(), sizeof(unsigned int));
        outputFile.write((char*)tile.getRegions(), sizeof(unsigned int)*1);
        outputFile.write((char*)&tile.getPopulation(), sizeof(double));
        outputFile.write((char*)&tile.getStoredGoods(), sizeof(float));
    }*/

    outputFile.close();
}

void Map::fromArray(const Array2<Tile::Type>& tiles)
{
    mHeight = tiles.getHeight();
    mWidth = tiles.getWidth();
    mTiles.reshape(mHeight, mWidth);
    for (unsigned int i = 0; i < mHeight; ++i)
    {
        for (unsigned int j = 0; j < mWidth; ++j)
        {
            mTiles.set(i, j, createTile(tiles.get(i, j)));
            mTiles.get(i, j)->setPosition(computePosition(i, j));
        }
    }

    for (unsigned int i = 0; i < mHeight; ++i)
    {
        for (unsigned int j = 0; j < mWidth; ++j)
            updateTile(i, j);
    }

    mNetwork.reshape(mWidth, mHeight);
}

void Map::deselect()
{
    for (std::unique_ptr<Tile>& tile : mTiles.getData())
        tile->setState(Tile::State::DESELECTED);
    mNbSelected = 0;
}

void Map::bulldoze(Tile::Type type)
{
    for (unsigned int i = 0; i < mHeight; ++i)
    {
        for (unsigned int j = 0; j < mWidth; ++j)
        {
            if (mTiles.get(i, j)->getState() == Tile::State::SELECTED)
            {
                mTiles.set(i, j, createTile(type));
                mTiles.get(i, j)->setPosition(computePosition(i, j));
                updateNeighborhood(i, j);
                if (mTiles.get(i, j)->isRoad())
                    mNetwork.addRoad(i, j);
                else
                    mNetwork.removeRoad(i, j);
            }
        }
    }
}

void Map::select(sf::Vector2i start, sf::Vector2i end, Tile::Category mask)
{
    // Swap coordinates if necessary
    if (end.y < start.y)
        std::swap(start.y, end.y);
    if (end.x < start.x)
        std::swap(start.x, end.x);

    // Clamp in range
    start.x = std::max<int>(std::min<int>(start.x, mWidth - 1), 0);
    start.y = std::max<int>(std::min<int>(start.y, mHeight - 1), 0);
    end.x = std::max<int>(std::min<int>(end.x, mWidth - 1), 0);
    end.y = std::max<int>(std::min<int>(end.y, mHeight - 1), 0);

    for (int i = start.y; i <= end.y; ++i)
    {
        for (int j = start.x; j <= end.x; ++j)
        {
            // Check if the tile type is in the blacklist. If it is, mark it as
            // invalid, otherwise select it
            const std::unique_ptr<Tile>& tile = mTiles.get(i, j);
            if (any(tile->getCategory() & mask))
            {
                tile->setState(Tile::State::SELECTED);
                ++mNbSelected;
            }
            else
                tile->setState(Tile::State::INVALID);
        }
    }
}

Path Map::getPath(sf::Vector2i start, sf::Vector2i end) const
{
    std::vector<sf::Vector2i> coordinates = mNetwork.getPath(start, end);
    std::vector<Vector2f> points;
    for (std::size_t i = 0; i < coordinates.size(); ++i)
    {
        const sf::Vector2i& coords = coordinates[i];
        // offset
        constexpr float t = 0.15f;
        const sf::Vector2f xOffset = sf::Vector2f(-0.5f, 0.25f) * Tile::SIZE;
        const sf::Vector2f yOffset = sf::Vector2f(-0.5f, -0.25f) * Tile::SIZE;
        sf::Vector2i iOffset;
        if (i > 0)
            iOffset += coords - coordinates[i-1];
        if (i < coordinates.size() - 1)
            iOffset += coordinates[i+1] - coords;
        float denom = std::abs(iOffset.x) == 0 || std::abs(iOffset.y) == 0 ? std::max(std::abs(iOffset.x), std::abs(iOffset.y)) : 0.5f;
        sf::Vector2f offset = t * (xOffset * iOffset.x + yOffset * iOffset.y) / denom;
        //std::cout << iOffset.x << " " << iOffset.y << " " << denom << " " << offset << std::endl;
        points.push_back(computePosition(coords.y, coords.x) + sf::Vector2f(Tile::SIZE, Tile::SIZE * 0.5f) + offset);
    }
    return Path(points);
}

unsigned int Map::getWidth() const
{
    return mWidth;
}

unsigned int Map::getHeight() const
{
    return mHeight;
}

const Array2<std::unique_ptr<Tile>>& Map::getTiles() const
{
    return mTiles;
}

unsigned int Map::getNbSelected() const
{
    return mNbSelected;
}

std::unique_ptr<Tile> Map::createTile(Tile::Type type)
{
    return sTileAtlas[static_cast<int>(type)]->clone();
}

sf::Vector2f Map::computePosition(std::size_t i, std::size_t j) const
{
    sf::Vector2f position;
    position.x = (j - i) * Tile::SIZE + mWidth * Tile::SIZE;
    position.y = (j + i) * Tile::SIZE * 0.5f;
    return position;
}

void Map::updateTile(int i, int j)
{
    const Tile* neighbors[3][3];
    for (int di = -1; di <= 1; ++di)
    {
        for (int dj = -1; dj <= 1; ++dj)
        {
            if (i + di >= 0 && i + di < static_cast<int>(mHeight) &&
                j + dj >= 0 && j + dj < static_cast<int>(mWidth))
                neighbors[dj+1][di+1] = mTiles.get(i + di, j + dj).get();
            else
                neighbors[dj+1][di+1] = sTileAtlas[static_cast<int>(Tile::Type::VOID)].get();
        }
    }
    if (mTiles.get(i, j)->updateVariant(neighbors))
        updateNeighborhood(i, j);
}

void Map::updateNeighborhood(int i, int j)
{
    for (int di = -1; di <= 1; ++di)
    {
        for (int dj = -1; dj <= 1; ++dj)
        {
            if (i + di >= 0 && i + di < static_cast<int>(mHeight) &&
                j + dj >= 0 && j + dj < static_cast<int>(mWidth))
                updateTile(i + di, j + dj);
        }
    }
}
