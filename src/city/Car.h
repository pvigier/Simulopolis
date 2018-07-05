#pragma once

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
    Car(const std::string& model);

    static void setTextureManager(TextureManager* textureManager);
    static void setImageManager(ImageManager* imageManager);

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

    int mWidth;
    int mHeight;
    Kinematic mKinematic;
    SteeringBehaviors mSteering;
    sf::Sprite mSprite;
    const sf::Image& mMask;
    Person* mDriver;
};
