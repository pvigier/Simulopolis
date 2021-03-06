/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Tile.h"
#include "resource/TextureManager.h"
#include "resource/ImageManager.h"
#include "render/sprite_intersection.h"

TextureManager* Tile::sTextureManager = nullptr;
ImageManager* Tile::sImageManager = nullptr;

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
    else if (s == "CFBHousing")
        return Type::CFB_HOUSING;
    else if (s == "affordableHousing")
        return Type::AFFORDABLE_HOUSING;
    else if (s == "apartmentBuilding")
        return Type::APARTMENT_BUILDING;
    else if (s == "villa")
        return Type::VILLA;
    else if (s == "CFBIndustry")
        return Type::CFB_INDUSTRY;
    else if (s == "farm")
        return Type::FARM;
    else if (s == "factory")
        return Type::FACTORY;
    else if (s == "workshop")
        return Type::WORKSHOP;
    else if (s == "CFBBusiness")
        return Type::CFB_BUSINESS;
    else if (s == "grocery")
        return Type::GROCERY;
    else if (s == "mall")
        return Type::MALL;
    else if (s == "boutique")
        return Type::BOUTIQUE;
    else if (s == "CFBHospital")
        return Type::CFB_HOSPITAL;
    else if (s == "hospital")
        return Type::HOSPITAL;
    else if (s == "CFBPoliceStation")
        return Type::CFB_POLICE_STATION;
    else if (s == "policeStation")
        return Type::POLICE_STATION;
    else if (s == "CFBSchool")
        return Type::CFB_SCHOOL;
    else if (s == "school")
        return Type::SCHOOL;
    else if (s == "roadGrass")
        return Type::ROAD_GRASS;
    else if (s == "roadSidewalk")
        return Type::ROAD_SIDEWALK;
    else if (s == "bridge")
        return Type::BRIDGE;
    else
        return Type::EMPTY;
}

std::string Tile::typeToString(Tile::Type type)
{
    switch (type)
    {
        case Type::EMPTY:
            return "Void";
        case Type::GRASS:
            return "Grass";
        case Type::FOREST:
            return "Forest";
        case Type::DIRT:
            return "Dirt";
        case Type::WATER:
            return "Water";
        case Type::CFB_HOUSING:
            return "Call for bids (housing)";
        case Type::AFFORDABLE_HOUSING:
            return "Affordable housing";
        case Type::APARTMENT_BUILDING:
            return "Appartment building";
        case Type::VILLA:
            return "Villa";
        case Type::CFB_INDUSTRY:
            return "Call for bids (industry)";
        case Type::FARM:
            return "Farm";
        case Type::FACTORY:
            return "Factory";
        case Type::WORKSHOP:
            return "Workshop";
        case Type::CFB_BUSINESS:
            return "Call for bids (business)";
        case Type::GROCERY:
            return "Grocery";
        case Type::MALL:
            return "Mall";
        case Type::BOUTIQUE:
            return "Boutique";
        case Type::CFB_HOSPITAL:
            return "Call for bids (hospital)";
        case Type::HOSPITAL:
            return "Hospital";
        case Type::CFB_POLICE_STATION:
            return "Call for bids (police station)";
        case Type::POLICE_STATION:
            return "Police station";
        case Type::CFB_SCHOOL:
            return "Call for bids (school)";
        case Type::SCHOOL:
            return "School";
        case Type::ROAD_GRASS:
            return "Road";
        case Type::ROAD_SIDEWALK:
            return "Road with sidewalk";
        case Type::BRIDGE:
            return "Bridge";
        default:
            return "";
    }
}

Tile::Tile(const std::string& name, Tile::Type type, Tile::Category category) :
    mTextureName(std::move(name)), mMask(nullptr), mType(type), mCategory(category), mState(Tile::State::DESELECTED)
{
    setUp();
}

Tile::~Tile()
{

}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

bool Tile::intersect(const sf::Vector2f& position) const
{
    return sprite_intersect(mSprite, *mMask, position);
}

std::unique_ptr<Tile> Tile::clone() const
{
    return std::make_unique<Tile>(mTextureName, mType, mCategory);
}

void Tile::updateVariant(const Tile* /*neighbors*/[3][3])
{
    mSprite.setOrigin(sf::Vector2f(0.0f, mSprite.getTextureRect().height - TILE_HEIGHT));
}

sf::Vector2i Tile::getCoordinates() const
{
    return mCoordinates;
}

void Tile::setPosition(sf::Vector2i coordinates, sf::Vector2f position)
{
    mCoordinates = coordinates;
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

bool Tile::isBuilding() const
{
    return mCategory == Category::BUILDING;
}

bool Tile::isHousing() const
{
    switch (mType)
    {
        case Type::AFFORDABLE_HOUSING:
        case Type::APARTMENT_BUILDING:
        case Type::VILLA:
            return true;
        default:
            return false;
    }
}

bool Tile::isIndustry() const
{
    switch (mType)
    {
        case Type::FARM:
        case Type::FACTORY:
        case Type::WORKSHOP:
            return true;
        default:
            return false;
    }
}

bool Tile::isBusiness() const
{
    switch (mType)
    {
        case Type::GROCERY:
        case Type::MALL:
        case Type::BOUTIQUE:
            return true;
        default:
            return false;
    }
}

bool Tile::isService() const
{
    switch (mType)
    {
        case Type::HOSPITAL:
        case Type::POLICE_STATION:
        case Type::SCHOOL:
            return true;
        default:
            return false;
    }
}

bool Tile::isCallForBids() const
{
    return mCategory == Category::CALL_FOR_BIDS;
}

bool Tile::isRoad() const
{
    switch (mCategory)
    {
        case Category::ROAD:
        case Category::BRIDGE:
            return true;
        default:
            return false;
    }
}

bool Tile::hasSidewalk() const
{
    return mType == Type::ROAD_SIDEWALK;
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

sf::FloatRect Tile::getBounds() const
{
    return mSprite.getGlobalBounds();
}

void Tile::setUp()
{
    mSprite.setTexture(sTextureManager->getTexture(mTextureName));
    mMask = &sImageManager->getImage(mTextureName);
}
