#pragma once

#include <string>
#include <vector>
#include "ai/Path.h"
#include "Tile.h"
#include "Network.h"

class TextureManager;
class Company;

class Map
{
public:
    using TileAtlas = std::vector<std::unique_ptr<Tile>>;

    Map();

    static void loadTiles(const TextureManager& textureManager);
    static const TileAtlas& getTileAtlas();

    void load(const std::string& filename, unsigned int width, unsigned int height);
    void save(const std::string& filename);
    void fromArray(const Array2<Tile::Type>& tiles);

    void select(sf::Vector2i start, sf::Vector2i end, Tile::Category mask);
    void deselect();
    void bulldoze(Tile::Type type, Company& owner);

    Path getPath(sf::Vector2i start, sf::Vector2i end) const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    const Array2<std::unique_ptr<Tile>>& getTiles() const;
    unsigned int getNbSelected() const;

private:
    static TileAtlas sTileAtlas;
    unsigned int mWidth;
    unsigned int mHeight;
    Array2<std::unique_ptr<Tile>> mTiles;
    unsigned int mNbSelected;
    Network mNetwork;

    static std::unique_ptr<Tile> createTile(Tile::Type type);
    sf::Vector2f computePosition(std::size_t i, std::size_t j) const;

    void updateTile(int i, int j);
    void updateNeighborhood(int i, int j);
};
