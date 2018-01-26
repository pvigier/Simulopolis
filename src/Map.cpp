#include "Map.h"
#include <fstream>

Map::Map() : mWidth(0), mHeight(0), mTileSize(8)
{
    mNumRegions[0] = 1;
}

Map::Map(const std::string& filename, unsigned int width, unsigned int height, TileAtlas& tileAtlas) :
    mTileSize(8)
{
    load(filename, width, height, tileAtlas);
}

void Map::load(const std::string& filename, unsigned int width, unsigned int height, TileAtlas& tileAtlas)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);

    mWidth = width;
    mHeight = height;

    for (unsigned int pos = 0; pos < mWidth * mHeight; ++pos)
    {
        mResources.push_back(255);

        TileType type;
        inputFile.read((char*)&type, sizeof(type));
        switch (type)
        {
            case TileType::FOREST:
                mTiles.push_back(tileAtlas.at("forest"));
                break;
            case TileType::WATER:
                mTiles.push_back(tileAtlas.at("water"));
                break;
            case TileType::RESIDENTIAL:
                mTiles.push_back(tileAtlas.at("residential"));
                break;
            case TileType::COMMERCIAL:
                mTiles.push_back(tileAtlas.at("commercial"));
                break;
            case TileType::INDUSTRIAL:
                mTiles.push_back(tileAtlas.at("industrial"));
                break;
            case TileType::ROAD:
                mTiles.push_back(tileAtlas.at("road"));
                break;
            default:
                mTiles.push_back(tileAtlas.at("grass"));
                break;
        }
        Tile& tile = mTiles.back();
        inputFile.read((char*)&tile.getVariant(), sizeof(unsigned int));
        inputFile.read((char*)tile.getRegions(), sizeof(unsigned int)*1);
        inputFile.read((char*)&tile.getPopulation(), sizeof(double));
        inputFile.read((char*)&tile.getStoredGoods(), sizeof(float));
    }

    inputFile.close();
}

void Map::save(const std::string& filename)
{
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::binary);

    for(Tile& tile : mTiles)
    {
        outputFile.write((char*)&tile.getType(), sizeof(TileType));
        outputFile.write((char*)&tile.getVariant(), sizeof(unsigned int));
        outputFile.write((char*)tile.getRegions(), sizeof(unsigned int)*1);
        outputFile.write((char*)&tile.getPopulation(), sizeof(double));
        outputFile.write((char*)&tile.getStoredGoods(), sizeof(float));
    }

    outputFile.close();
}

void Map::draw(sf::RenderWindow& window, float dt)
{
    for(unsigned int y = 0; y < mHeight; ++y)
    {
        for(unsigned int x = 0; x < mWidth; ++x)
        {
            // Compute the position of the tile in the 2d world
            sf::Vector2f pos;
            pos.x = (x - y) * mTileSize + mWidth * mTileSize;
            pos.y = (x + y) * mTileSize * 0.5;
            mTiles[y * mWidth + x].getSprite().setPosition(pos);

            // Draw the tile
            mTiles[y * mWidth + x].draw(window, dt);
        }
    }
    return;
}

void Map::findConnectedRegions(std::vector<TileType> whitelist, int regionType)
{
    int label = 1;

    // Reset the label of all tiles
    for (Tile& tile : mTiles)
        tile.getRegions()[regionType] = 0;

    for (unsigned int y = 0; y < mHeight; ++y)
    {
        for (unsigned int x = 0; x < mWidth; ++x)
        {
            // Remove this test?
            bool found = false;
            for (TileType type : whitelist)
            {
                if(type == mTiles[y * mWidth + x].getType())
                {
                    found = true;
                    break;
                }
            }
            if(mTiles[y * mWidth + x].getRegions()[regionType] == 0 && found)
                depthFirstSearch(whitelist, x, y, label++, regionType);
        }
    }
    mNumRegions[regionType] = label;
}

void Map::updateDirection(TileType type)
{
    for (unsigned int y = 0; y < mHeight; ++y)
    {
        for (unsigned int x = 0; x < mWidth; ++x)
        {
            int pos = y * mWidth + x;

            if (mTiles[pos].getType() != type)
                continue;

            bool adjacentTiles[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

            // Check for adjacent tiles of the same type
            if (x > 0 && y > 0)
                adjacentTiles[0][0] = (mTiles[(y - 1) * mWidth + (x - 1)].getType() == type);
            if (y > 0)
                adjacentTiles[0][1] = (mTiles[(y - 1) * mWidth + x].getType() == type);
            if (x < mWidth - 1 && y > 0)
                adjacentTiles[0][2] = (mTiles[(y - 1) * mWidth + (x + 1)].getType() == type);
            if (x > 0)
                adjacentTiles[1][0] = (mTiles[y * mWidth + (x-1)].getType() == type);
            if (x < mWidth - 1)
                adjacentTiles[1][2] = (mTiles[y * mWidth + (x + 1)].getType() == type);
            if (x > 0 && y < mHeight - 1)
                adjacentTiles[2][0] = (mTiles[(y + 1) * mWidth + (x - 1)].getType() == type);
            if (y < mHeight - 1)
                adjacentTiles[2][1] = (mTiles[(y + 1) * mWidth + x].getType() == type);
            if (x < mWidth-1 && y < mHeight-1)
                adjacentTiles[2][2] = (mTiles[(y + 1) * mWidth + (x + 1)].getType() == type);

            // Change the tile variant depending on the tile position
            if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1])
                mTiles[pos].getVariant() = 2;
            else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1])
                mTiles[pos].getVariant() = 7;
            else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1])
                mTiles[pos].getVariant() = 8;
            else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0])
                mTiles[pos].getVariant() = 9;
            else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2])
                mTiles[pos].getVariant() = 10;
            else if (adjacentTiles[1][0] && adjacentTiles[1][2])
                mTiles[pos].getVariant() = 0;
            else if (adjacentTiles[0][1] && adjacentTiles[2][1])
                mTiles[pos].getVariant() = 1;
            else if (adjacentTiles[2][1] && adjacentTiles[1][0])
                mTiles[pos].getVariant() = 3;
            else if (adjacentTiles[0][1] && adjacentTiles[1][2])
                mTiles[pos].getVariant() = 4;
            else if (adjacentTiles[1][0] && adjacentTiles[0][1])
                mTiles[pos].getVariant() = 5;
            else if (adjacentTiles[2][1] && adjacentTiles[1][2])
                mTiles[pos].getVariant() = 6;
            else if (adjacentTiles[1][0])
                mTiles[pos].getVariant() = 0;
            else if (adjacentTiles[1][2])
                mTiles[pos].getVariant() = 0;
            else if (adjacentTiles[0][1])
                mTiles[pos].getVariant() = 1;
            else if (adjacentTiles[2][1])
                mTiles[pos].getVariant() = 1;
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

unsigned int Map::getTileSize() const
{
    return mTileSize;
}

void Map::depthFirstSearch(std::vector<TileType>& whitelist, int x, int y, int label, int regionType)
{
    // Outside of the map
    if (x < 0 || x >= static_cast<int>(mWidth) || y < 0 || y >= static_cast<int>(mHeight))
        return;

    // Check if the tile is already assigned to a region
    if (mTiles[y * mWidth + x].getRegions()[regionType] != 0)
        return;

    // Check if the type of the tile is in the whitelist
    bool found = false;
    for (TileType type : whitelist)
    {
        if (type == mTiles[y * mWidth + x].getType())
        {
            found = true;
            break;
        }
    }
    if (!found)
        return;

    // Label the tile
    mTiles[y * mWidth + x].getRegions()[regionType] = label;

    // Recursive calls
    depthFirstSearch(whitelist, x - 1, y, label, regionType);
    depthFirstSearch(whitelist, x, y + 1, label, regionType);
    depthFirstSearch(whitelist, x + 1, y, label, regionType);
    depthFirstSearch(whitelist, x, y - 1, label, regionType);
}
