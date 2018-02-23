#include "Map.h"
#include <fstream>
#include "resource/TextureManager.h"
#include "city/Road.h"

std::vector<std::unique_ptr<Tile>> Map::sTileAtlas;

Map::Map() : mWidth(0), mHeight(0), mNumSelected(0)
{
    mNumRegions[0] = 1;
}

Map::Map(const std::string& filename, unsigned int width, unsigned int height) :
    mNumSelected(0)
{
    load(filename, width, height);
}

void Map::loadTiles(const TextureManager& textureManager)
{
    // void
    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("grass"),
        sf::IntRect(0, 0, 132, 101), Tile::Type::GRASS, 1)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("grass"),
        sf::IntRect(0, 0, 132, 101), Tile::Type::GRASS, 1)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("forest"),
        sf::IntRect(0, 0, 132, 101), Tile::Type::FOREST, 1)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("water"),
        sf::IntRect(0, 0, 132, 101), Tile::Type::WATER, 1)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("residential"),
        sf::IntRect(0, 0, 132, 165), Tile::Type::RESIDENTIAL, 2)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("commercial"),
        sf::IntRect(0, 0, 132, 165), Tile::Type::COMMERCIAL, 2)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Tile(textureManager.getTexture("industrial"),
        sf::IntRect(0, 0, 132, 165), Tile::Type::INDUSTRIAL, 2)));

    sTileAtlas.push_back(std::unique_ptr<Tile>(new Road(textureManager.getTexture("road"),
        sf::IntRect(0, 0, 132, 101), Tile::Type::ROAD, 1)));
}

void Map::load(const std::string& filename, unsigned int width, unsigned int height)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);

    mWidth = width;
    mHeight = height;

    for (unsigned int pos = 0; pos < mWidth * mHeight; ++pos)
    {
        mTileStates.push_back(Tile::State::DESELECTED);

        Tile::Type type;
        inputFile.read((char*)&type, sizeof(type));
        mTiles.push_back(createTile(type));
        char tmp[4];
        inputFile.read(tmp, sizeof(unsigned int));
        inputFile.read(tmp, sizeof(unsigned int));
        inputFile.read(tmp, sizeof(double));
        inputFile.read(tmp, sizeof(float));
    }

    inputFile.close();
}

void Map::save(const std::string& filename)
{
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::binary);

    for(std::unique_ptr<Tile>& tile : mTiles)
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

void Map::draw(sf::RenderWindow& window)
{
    for(unsigned int y = 0; y < mHeight; ++y)
    {
        for(unsigned int x = 0; x < mWidth; ++x)
        {
            sf::Sprite& sprite = mTiles[y * mWidth + x]->getSprite();
            // Compute the position of the tile in the 2d world
            sf::Vector2f pos;
            pos.x = (x - y) * Tile::SIZE + mWidth * Tile::SIZE;
            pos.y = (x + y) * Tile::SIZE * 0.5f;
            sprite.setPosition(pos);

            // Change the color if the tile is selected
            if(mTileStates[y * mWidth + x] == Tile::State::SELECTED)
                sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
            else
                sprite.setColor(sf::Color(0xff, 0xff, 0xff));

            // Draw the tile
            window.draw(sprite);
        }
    }
    return;
}

void Map::deselect()
{
    for (Tile::State& state : mTileStates)
        state = Tile::State::DESELECTED;

    mNumSelected = 0;
}

void Map::bulldoze(Tile::Type type)
{
    for (std::size_t i = 0; i < mTiles.size(); ++i)
    {
        if (mTileStates[i] == Tile::State::SELECTED)
        {
            mTiles[i] = createTile(type);
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
            Tile::Type type = mTiles[y * mWidth + x]->getType();
            if (std::find(blacklist.begin(), blacklist.end(), type) == blacklist.end())
            {
                mTileStates[y * mWidth + x] = Tile::State::SELECTED;
                ++mNumSelected;
            }
            else
                mTileStates[y * mWidth + x] = Tile::State::INVALID;
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

unsigned int Map::getNbTiles() const
{
    return mTiles.size();
}

Tile::State Map::getTileState(std::size_t position) const
{
    return mTileStates[position];
}

unsigned int Map::getNumSelected() const
{
    return mNumSelected;
}

std::unique_ptr<Tile> Map::createTile(Tile::Type type)
{
    return sTileAtlas[static_cast<int>(type)]->clone();
}

void Map::updateTile(int pos)
{
    Tile::Type neighbors[3][3];
    for (int dy = 0; dy < 3; ++dy)
    {
        for (int dx = 0; dx < 3; ++dx)
        {
            int neighborPos = pos + (dy - 1) * mWidth + (dx - 1);
            if (neighborPos >= 0 && static_cast<std::size_t>(neighborPos) < mTiles.size())
                neighbors[dx][dy] = mTiles[neighborPos]->getType();
            else
                neighbors[dx][dy] = Tile::Type::VOID;
        }
    }
    mTiles[pos]->updateVariant(neighbors);
}

void Map::updateNeighborhood(std::size_t pos)
{
    for (int dy = 0; dy < 3; ++dy)
    {
        for (int dx = 0; dx < 3; ++dx)
        {
            int neighborPos = pos + (dy - 1) * mWidth + (dx - 1);
            if (neighborPos >= 0 && static_cast<std::size_t>(neighborPos) < mTiles.size())
                updateTile(neighborPos);
        }
    }
}
