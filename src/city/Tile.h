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
 
#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <SFML/Graphics.hpp>
#include "util/enum.h"

class TextureManager;
class ImageManager;

class Tile : public sf::Drawable
{
public:
    enum class Category : unsigned int {GROUND = 1, WATER = 2, BUILDING = 4, ROAD = 8, BRIDGE = 16,
        OBSTACLE = 32, CALL_FOR_BIDS = 64};
    enum class Type : int {VOID, GRASS, FOREST, WATER, DIRT,
        CFB_HOUSING, AFFORDABLE_HOUSING, APARTMENT_BUILDING, VILLA,
        CFB_INDUSTRY, FARM, FACTORY, WORKSHOP,
        CFB_BUSINESS, GROCERY, MALL, BOUTIQUE,
        CFB_HOSPITAL, HOSPITAL,
        CFB_POLICE_STATION, POLICE_STATION,
        CFB_SCHOOL, SCHOOL,
        ROAD_GRASS, ROAD_SIDEWALK, BRIDGE};
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

    virtual void updateVariant(const Tile* neighbors[3][3]);

    sf::Vector2i getCoordinates() const;
    virtual void setPosition(sf::Vector2i coordinates, sf::Vector2f position);
    Category getCategory() const;
    Type getType() const;
    bool isBuilding() const;
    bool isHousing() const;
    bool isIndustry() const;
    bool isBusiness() const;
    bool isService() const;
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
    const sf::Image* mMask;
    Type mType;
    Category mCategory;
    State mState;

    Tile() = default;

private:
    void setUp();

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
        ar & mTextureName & mCoordinates & mType & mCategory & mState;
    }

    template<typename Archive>
    void load(Archive& ar, const unsigned int /*version*/)
    {
        ar & mTextureName & mCoordinates & mType & mCategory & mState;
        setUp();
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

ENABLE_BITMASK_OPERATORS(Tile::Category)
