#pragma once

#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Map
{
public:
    Map();
    Map(const std::string& filename, unsigned int width, unsigned int height, TileAtlas& tileAtlas);

    void load(const std::string& filename, unsigned int width, unsigned int height, TileAtlas& tileAtlas);
    void save(const std::string& filename);

    void draw(sf::RenderWindow& window, float dt);

    void findConnectedRegions(std::vector<TileType> whitelist, int regionType = 0);
    void updateDirection(TileType type);

        // Select the tiles within the bounds
    void select(sf::Vector2i start, sf::Vector2i end, std::vector<TileType> blacklist);
    // Deselect all tiles
    void clearSelected();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    const Tile& getTile(std::size_t position) const;
    void setTile(std::size_t position, Tile tile);
    unsigned int getTileSize() const;
    void setTileSize(unsigned int tileSize);
    TileState getTileState(std::size_t position) const;

private:
    unsigned int mWidth;
    unsigned int mHeight;
    std::vector<Tile> mTiles;
    std::vector<int> mResources;
    unsigned int mTileSize;
    unsigned int mSumSelected;
    unsigned int mNumRegions[1];
    std::vector<TileState> mTileStates;
    unsigned int mNumSelected;

    void depthFirstSearch(std::vector<TileType>& whitelist, int x, int y, int label, int regionType);
};
