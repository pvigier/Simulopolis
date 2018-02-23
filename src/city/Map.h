#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class TextureManager;

class Map
{
public:

    Map();
    Map(const std::string& filename, unsigned int width, unsigned int height);

    static void loadTiles(const TextureManager& textureManager);
    static Tile createTile(Tile::Type type);

    void load(const std::string& filename, unsigned int width, unsigned int height);
    void save(const std::string& filename);

    void draw(sf::RenderWindow& window);

    void select(sf::Vector2i start, sf::Vector2i end, const std::vector<Tile::Type>& blacklist);
    void deselect();
    void bulldoze(Tile::Type type);

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getNbTiles() const;
    Tile::State getTileState(std::size_t position) const;
    unsigned int getNumSelected() const;

private:
    static std::vector<std::unique_ptr<Tile>> sTileAtlas;
    unsigned int mWidth;
    unsigned int mHeight;
    std::vector<std::unique_ptr<Tile>> mTiles;
    unsigned int mSumSelected;
    unsigned int mNumRegions[1];
    std::vector<Tile::State> mTileStates;
    unsigned int mNumSelected;
};
