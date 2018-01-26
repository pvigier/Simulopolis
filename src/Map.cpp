#include "Map.h"
#include <fstream>

Map::Map() : mWidth(0), mHeight(0), mTileSize(8)
{
    mNumRegions[0] = 1;
}

Map::Map(const std::string& filename, unsigned int width, unsigned int height,
    std::map<std::string, Tile>& tileAtlas) : mTileSize(8)
{
    load(filename, width, height, tileAtlas);
}

void Map::load(const std::string& filename, unsigned int width, unsigned int height,
    std::map<std::string, Tile>& tileAtlas)
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
