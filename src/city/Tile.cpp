#include "Tile.h"

Tile::Tile(const sf::Texture& texture, Tile::Type type, unsigned int height) :
    mSprite(texture), mType(type), mState(Tile::State::DESELECTED)
{
    mSprite.setOrigin(sf::Vector2f(0.0f, Tile::SIZE * (height - 1)));
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
    else if (s == "road")
        return Tile::Type::ROAD;
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
        case Tile::Type::ROAD:
            return "Road";
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
    mSprite.setTextureRect(sf::IntRect(0, 0, 132, 101));
    return false;
}

bool Tile::isBuilding() const
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
