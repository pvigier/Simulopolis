#include "Map.h"
#include <fstream>
#include "resource/TextureManager.h"
#include "city/Road.h"
#include "city/Building.h"

std::vector<std::unique_ptr<Tile>> Map::sTileAtlas;

Map::Map() : mWidth(0), mHeight(0), mNbSelected(0)
{

}

Map::Map(const std::string& filename, unsigned int width, unsigned int height) :
    mNbSelected(0)
{
    load(filename, width, height);
}

void Map::loadTiles(const TextureManager& textureManager)
{
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("grass"),
        Tile::Type::VOID)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("grass"),
        Tile::Type::GRASS)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("forest"),
         Tile::Type::FOREST)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("water"),
        Tile::Type::WATER)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building(textureManager.getTexture("residential"),
        Tile::Type::RESIDENTIAL)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building(textureManager.getTexture("commercial"),
        Tile::Type::COMMERCIAL)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Building(textureManager.getTexture("industrial"),
        Tile::Type::INDUSTRIAL)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Road(textureManager.getTexture("road"),
        Tile::Type::ROAD)));
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (unsigned int y = 0; y < mHeight; ++y)
    {
        for (unsigned int x = 0; x < mWidth; ++x)
            target.draw(*mTiles[y * mWidth + x]);
    }
}

void Map::load(const std::string& filename, unsigned int width, unsigned int height)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);

    mWidth = width;
    mHeight = height;

    for (unsigned int i = 0; i < mWidth * mHeight; ++i)
    {
        Tile::Type type;
        inputFile.read((char*)&type, sizeof(type));
        mTiles.push_back(createTile(type));
        mTiles.back()->setPosition(computePosition(mTiles.size() - 1));
        char tmp[4];
        inputFile.read(tmp, sizeof(unsigned int));
        inputFile.read(tmp, sizeof(unsigned int));
        inputFile.read(tmp, sizeof(double));
        inputFile.read(tmp, sizeof(float));
    }

    inputFile.close();

    for (std::size_t i = 0; i < mTiles.size(); ++i)
        updateTile(i);
}

void Map::save(const std::string& filename)
{
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::binary);

    for (const std::unique_ptr<Tile>& tile : mTiles)
    {
        Tile::Type type = tile->getType();
        outputFile.write((char*)&type, sizeof(Tile::Type));
        /*outputFile.write((char*)&tile.getVariant(), sizeof(unsigned int));
        outputFile.write((char*)tile.getRegions(), sizeof(unsigned int)*1);
        outputFile.write((char*)&tile.getPopulation(), sizeof(double));
        outputFile.write((char*)&tile.getStoredGoods(), sizeof(float));*/
    }

    outputFile.close();
}

void Map::deselect()
{
    for (std::unique_ptr<Tile>& tile : mTiles)
        tile->setState(Tile::State::DESELECTED);
    mNbSelected = 0;
}

void Map::bulldoze(Tile::Type type)
{
    for (std::size_t i = 0; i < mTiles.size(); ++i)
    {
        if (mTiles[i]->getState() == Tile::State::SELECTED)
        {
            mTiles[i] = createTile(type);
            mTiles[i]->setPosition(computePosition(i));
            updateNeighborhood(i);
        }
    }
}

void Map::select(sf::Vector2i start, sf::Vector2i end, const std::vector<Tile::Type>& blacklist)
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

    for (int y = start.y; y <= end.y; ++y)
    {
        for (int x = start.x; x <= end.x; ++x)
        {
            // Check if the tile type is in the blacklist. If it is, mark it as
            // invalid, otherwise select it
            const std::unique_ptr<Tile>& tile =mTiles[y * mWidth + x];
            if (std::find(blacklist.begin(), blacklist.end(), tile->getType()) == blacklist.end())
            {
                tile->setState(Tile::State::SELECTED);
                ++mNbSelected;
            }
            else
                tile->setState(Tile::State::INVALID);
        }
    }
}

unsigned int Map::getWidth() const
{
    return mWidth;
}

unsigned int Map::getHeight() const
{
    return mHeight;
}

unsigned int Map::getNbSelected() const
{
    return mNbSelected;
}

std::unique_ptr<Tile> Map::createTile(Tile::Type type)
{
    return sTileAtlas[static_cast<int>(type)]->clone();
}

sf::Vector2f Map::computePosition(std::size_t i)
{
    int x = i % mWidth;
    int y = i / mWidth;
    sf::Vector2f position;
    position.x = (x - y) * Tile::SIZE + mWidth * Tile::SIZE;
    position.y = (x + y) * Tile::SIZE * 0.5f;
    return position;
}

void Map::updateTile(int i)
{
    Tile* neighbors[3][3];
    for (int dy = 0; dy < 3; ++dy)
    {
        for (int dx = 0; dx < 3; ++dx)
        {
            int iNeighbor = i + (dy - 1) * mWidth + (dx - 1);
            if (iNeighbor >= 0 && static_cast<std::size_t>(iNeighbor) < mTiles.size())
                neighbors[dx][dy] = mTiles[iNeighbor].get();
            else
                neighbors[dx][dy] = sTileAtlas[static_cast<int>(Tile::Type::VOID)].get();
        }
    }
    if (mTiles[i]->updateVariant(neighbors))
        updateNeighborhood(i);
}

void Map::updateNeighborhood(std::size_t i)
{
    for (int dy = 0; dy < 3; ++dy)
    {
        for (int dx = 0; dx < 3; ++dx)
        {
            int iNeighbor = i + (dy - 1) * mWidth + (dx - 1);
            if (iNeighbor >= 0 && static_cast<std::size_t>(iNeighbor) < mTiles.size())
                updateTile(iNeighbor);
        }
    }
}
