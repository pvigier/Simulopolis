#include "Tile.h"

Tile::Tile(const sf::Texture& texture, Tile::Type type) :
    mSprite(texture), mType(type), mState(Tile::State::DESELECTED)
{

}

Tile::~Tile()
{

}

Tile::Type Tile::stringToType(const std::string& s)
{
    if (s == "grass")
        return Tile::Type::GRASS;
    else if (s == "forest")
        return Tile::Type::FOREST;
    else if (s == "water")
        return Tile::Type::WATER;
    else if (s == "residential")
        return Tile::Type::RESIDENTIAL;
    else if (s == "commercial")
        return Tile::Type::COMMERCIAL;
    else if (s == "industrial")
        return Tile::Type::INDUSTRIAL;
    else if (s == "hospital")
        return Tile::Type::HOSPITAL;
    else if (s == "police")
        return Tile::Type::POLICE;
    else if (s == "school")
        return Tile::Type::SCHOOL;
    else if (s == "roadGrass")
        return Tile::Type::ROAD_GRASS;
    else if (s == "roadSidewalk")
        return Tile::Type::ROAD_SIDEWALK;
    else if (s == "roadWater")
        return Tile::Type::ROAD_WATER;
    else
        return Tile::Type::VOID;
}

std::string Tile::typeToString(Tile::Type type)
{
    switch (type)
    {
        case Tile::Type::VOID:
            return "Void";
        case Tile::Type::GRASS:
            return "Grass";
        case Tile::Type::FOREST:
            return "Forest";
        case Tile::Type::WATER:
            return "Water";
        case Tile::Type::RESIDENTIAL:
            return "Residential Zone";
        case Tile::Type::COMMERCIAL:
            return "Commercial Zone";
        case Tile::Type::INDUSTRIAL:
            return "Industrial Zone";
        case Tile::Type::HOSPITAL:
            return "Hospital";
        case Tile::Type::POLICE:
            return "Police";
        case Tile::Type::SCHOOL:
            return "School";
        case Tile::Type::ROAD_GRASS:
            return "Road";
        case Tile::Type::ROAD_SIDEWALK:
            return "Road with sidewalk";
        case Tile::Type::ROAD_WATER:
            return "Road on water";
        default:
            return "";
    }
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite);
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
    return false;
}

bool Tile::isRoad() const
{
    return false;
}

bool Tile::hasSidewalk() const
{
    return false;
}

void Tile::setPosition(sf::Vector2f position)
{
    mSprite.setPosition(position);
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
