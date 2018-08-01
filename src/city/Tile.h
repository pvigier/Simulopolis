#pragma once

#include <SFML/Graphics.hpp>
#include "util/enum.h"

class TextureManager;
class ImageManager;

class Tile : public sf::Drawable
{
public:
    enum class Category : unsigned int {GROUND = 1, WATER = 2, BUILDING = 4, ROAD = 8, OBSTACLE = 16,
        CALL_FOR_BIDS = 32};
    enum class Type : int {VOID, GRASS, FOREST, WATER, DIRT,
        CFB_HOUSING, AFFORDABLE_HOUSING, APARTMENT_BUILDING, VILLA,
        CFB_INDUSTRY, FARM, FACTORY, WORKSHOP,
        CFB_BUSINESS, MARKET, MALL, BOUTIQUE,
        CFB_HOSPITAL, HOSPITAL,
        CFB_POLICE_STATION, POLICE_STATION,
        CFB_SCHOOL, SCHOOL,
        ROAD_GRASS, ROAD_SIDEWALK, ROAD_WATER};
    enum class State{DESELECTED, SELECTED, INVALID};
    static constexpr unsigned int SIZE = 64;
    static constexpr float TILE_HEIGHT = 101.f;

    static void setTextureManager(TextureManager* textureManager);
    static void setImageManager(ImageManager* imageManager);

    static Type stringToType(const std::string& s);
    static std::string typeToString(Type type);

    Tile(const std::string& name, Type type, Category category);
    virtual ~Tile();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual bool intersect(const sf::Vector2f& position) const;

    virtual std::unique_ptr<Tile> clone() const;

    virtual bool updateVariant(const Tile* neighbors[3][3]);

    sf::Vector2i getCoordinates() const;
    virtual void setPosition(sf::Vector2i coordinates, sf::Vector2f position);
    Category getCategory() const;
    Type getType() const;
    bool isBuilding() const;
    bool isHousing() const;
    bool isIndustry() const;
    bool isBusiness() const;
    bool isCallForBids() const;
    bool isRoad() const;
    bool hasSidewalk() const;
    State getState() const;
    void setState(State state);
    virtual sf::FloatRect getBounds() const;

protected:
    static TextureManager* sTextureManager;
    static ImageManager* sImageManager;

    std::string mTextureName;
    sf::Sprite mSprite;
    sf::Vector2i mCoordinates;
    const sf::Image& mMask;
    Type mType;
    Category mCategory;
    State mState;
};

ENABLE_BITMASK_OPERATORS(Tile::Category);
