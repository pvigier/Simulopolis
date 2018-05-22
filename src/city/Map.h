#pragma once

#include <string>
#include <vector>
#include "util/Array2.h"
#include "ai/Path.h"
#include "Tile.h"
#include "Network.h"

class TextureManager;

class Map
{
public:

    Map();
    Map(const std::string& filename, unsigned int width, unsigned int height);

    static void loadTiles(const TextureManager& textureManager);

    void load(const std::string& filename, unsigned int width, unsigned int height);
    void save(const std::string& filename);

    void select(sf::Vector2i start, sf::Vector2i end, const std::vector<Tile::Type>& blacklist);
    void deselect();
    void bulldoze(Tile::Type type);

    Path getPath(sf::Vector2i start, sf::Vector2i end) const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    const Array2<std::unique_ptr<Tile>>& getTiles() const;
    unsigned int getNbSelected() const;

private:
    static std::vector<std::unique_ptr<Tile>> sTileAtlas;
    unsigned int mWidth;
    unsigned int mHeight;
    Array2<std::unique_ptr<Tile>> mTiles;
    unsigned int mNbSelected;
    Network mNetwork;

    static std::unique_ptr<Tile> createTile(Tile::Type type);
    sf::Vector2f computePosition(std::size_t i, std::size_t j) const;

    void updateTile(int i, int j);
    void updateNeighborhood(std::size_t i, std::size_t j);
};
