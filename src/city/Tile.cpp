#include "Tile.h"
#include "resource/TextureManager.h"
#include "resource/ImageManager.h"
#include "render/sprite_intersection.h"

TextureManager* Tile::sTextureManager = nullptr;
ImageManager* Tile::sImageManager = nullptr;

Tile::Tile(const std::string& name, Tile::Type type, Tile::Category category) :
    mSprite(sTextureManager->getTexture(name)), mMask(sImageManager->getImage(name)),
    mType(type), mCategory(category), mState(Tile::State::DESELECTED)
{

}

Tile::~Tile()
{

}

void Tile::setTextureManager(TextureManager* textureManager)
{
    sTextureManager = textureManager;
}

void Tile::setImageManager(ImageManager* imageManager)
{
    sImageManager = imageManager;
}

Tile::Type Tile::stringToType(const std::string& s)
{
    if (s == "grass")
        return Type::GRASS;
    else if (s == "forest")
        return Type::FOREST;
    else if (s == "dirt")
        return Type::DIRT;
    else if (s == "water")
        return Type::WATER;
    else if (s == "affordableHousing")
        return Type::AFFORDABLE_HOUSING;
    else if (s == "apartmentBuilding")
        return Type::APARTMENT_BUILDING;
    else if (s == "villa")
        return Type::VILLA;
    else if (s == "farm")
        return Type::FARM;
    else if (s == "factory")
        return Type::FACTORY;
    else if (s == "workshop")
        return Type::WORKSHOP;
    else if (s == "market")
        return Type::MARKET;
    else if (s == "mall")
        return Type::MALL;
    else if (s == "boutique")
        return Type::BOUTIQUE;
    else if (s == "hospital")
        return Type::HOSPITAL;
    else if (s == "police")
        return Type::POLICE;
    else if (s == "school")
        return Type::SCHOOL;
    else if (s == "roadGrass")
        return Type::ROAD_GRASS;
    else if (s == "roadSidewalk")
        return Type::ROAD_SIDEWALK;
    else if (s == "roadWater")
        return Type::ROAD_WATER;
    else
        return Type::VOID;
}

std::string Tile::typeToString(Tile::Type type)
{
    switch (type)
    {
        case Type::VOID:
            return "Void";
        case Type::GRASS:
            return "Grass";
        case Type::FOREST:
            return "Forest";
        case Type::DIRT:
            return "Dirt";
        case Type::WATER:
            return "Water";
        case Type::AFFORDABLE_HOUSING:
            return "Affordable housing";
        case Type::APARTMENT_BUILDING:
            return "Appartment building";
        case Type::VILLA:
            return "Villa";
        case Type::FARM:
            return "Farm";
        case Type::FACTORY:
            return "Factory";
        case Type::WORKSHOP:
            return "Workshop";
        case Type::MARKET:
            return "Market";
        case Type::MALL:
            return "Mall";
        case Type::BOUTIQUE:
            return "Boutique";
        case Type::HOSPITAL:
            return "Hospital";
        case Type::POLICE:
            return "Police";
        case Type::SCHOOL:
            return "School";
        case Type::ROAD_GRASS:
            return "Road";
        case Type::ROAD_SIDEWALK:
            return "Road with sidewalk";
        case Type::ROAD_WATER:
            return "Road on water";
        default:
            return "";
    }
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite);
}

bool Tile::intersect(const sf::Vector2f& position) const
{
    return sprite_intersect(mSprite, mMask, position);
}

std::unique_ptr<Tile> Tile::clone() const
{
    return std::unique_ptr<Tile>(new Tile(*this));
}

bool Tile::updateVariant(Tile* neighbors[3][3])
{
    mSprite.setOrigin(sf::Vector2f(0.0f, mSprite.getTextureRect().height - TILE_HEIGHT));
    return false;
}

bool Tile::isBuilding() const
{
    return mCategory == Category::BUILDING;
}

bool Tile::isRoad() const
{
    return mCategory == Category::ROAD;
}

bool Tile::hasSidewalk() const
{
    return mType == Type::ROAD_SIDEWALK;
}

void Tile::setPosition(sf::Vector2f position)
{
    mSprite.setPosition(position);
}

Tile::Category Tile::getCategory() const
{
    return mCategory;
}

Tile::Type Tile::getType() const
{
    return mType;
}

Tile::State Tile::getState() const
{
    return mState;
}

void Tile::setState(Tile::State state)
{
    mState = state;
    // Change the color if the tile is selected
    if(mState == Tile::State::SELECTED)
        mSprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
    else
        mSprite.setColor(sf::Color(0xff, 0xff, 0xff));
}
