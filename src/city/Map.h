#pragma once

#include <string>
#include <vector>
#include "util/Id.h"
#include "ai/Path.h"
#include "city/Tile.h"
#include "city/Network.h"

class TextureManager;
class Company;
class Building;
template<typename T> class IdManager;

class Map
{
public:
    using TileAtlas = std::vector<std::unique_ptr<Tile>>;

    Map();

    static void loadTiles();
    static const TileAtlas& getTileAtlas();

    // IO
    void load(const std::string& filename, unsigned int width, unsigned int height);
    void save(const std::string& filename);
    void fromArray(const Array2<Tile::Type>& tiles);

    // Modifying the map
    void select(sf::Vector2i start, sf::Vector2i end, Tile::Category mask);
    void deselect();
    void bulldoze(Tile::Type type, Company& owner, IdManager<Building*>& buildings, std::vector<Id>& buildingsToRemove);

    // Network
    const Network& getNetwork() const;
    bool isReachableFrom(const Tile* start, const Tile* end) const;
    Path getPath(sf::Vector2i start, sf::Vector2i end) const;
    std::vector<const Building*> getReachableBuildingsAround(const Tile* origin, int radius, Tile::Type type) const;

    // Data
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    const Tile* getTile(std::size_t i, std::size_t j) const;
    const Array2<std::unique_ptr<Tile>>& getTiles() const;
    unsigned int getNbSelected() const;

    // Util
    sf::Vector2f computePosition(std::size_t i, std::size_t j) const;

private:
    static TileAtlas sTileAtlas;
    unsigned int mWidth;
    unsigned int mHeight;
    Array2<std::unique_ptr<Tile>> mTiles;
    unsigned int mNbSelected;
    Network mNetwork;

    static std::unique_ptr<Tile> createTile(Tile::Type type);

    void updateTile(int i, int j);
    void updateNeighborhood(int i, int j);
    void updateTiles();

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
        ar & mWidth & mHeight & mTiles & mNbSelected & mNetwork;
    }

    template<typename Archive>
    void load(Archive& ar, const unsigned int /*version*/)
    {
        ar & mWidth & mHeight & mTiles & mNbSelected & mNetwork;
        updateTiles();
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};
