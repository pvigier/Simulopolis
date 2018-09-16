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

#include <boost/serialization/split_member.hpp>
#include <SFML/Graphics.hpp>
#include "ai/Kinematic.h"
#include "ai/SteeringBehaviors.h"
#include "render/sprite_intersection.h"

class TextureManager;
class ImageManager;
class Person;

class Car : public sf::Drawable
{
public:
    static void setTextureManager(TextureManager* textureManager);
    static void setImageManager(ImageManager* imageManager);

    Car() = default;
    Car(std::string model);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);
    bool intersect(const sf::Vector2f& position) const;

    Kinematic& getKinematic();
    const Kinematic& getKinematic() const;
    SteeringBehaviors& getSteering();
    sf::FloatRect getBounds() const;
    Person* getDriver();
    const Person* getDriver() const;
    void setDriver(Person* owner);

private:
    static TextureManager* sTextureManager;
    static ImageManager* sImageManager;

    std::string mModel;
    int mWidth;
    int mHeight;
    Kinematic mKinematic;
    SteeringBehaviors mSteering;
    sf::Sprite mSprite;
    const sf::Image* mMask;
    Person* mDriver;

    void setUp();

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
        ar & mModel & mKinematic & mSteering & mDriver;
    }

    template<typename Archive>
    void load(Archive& ar, const unsigned int /*version*/)
    {
        ar & mModel & mKinematic & mSteering & mDriver;
        setUp();
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};
