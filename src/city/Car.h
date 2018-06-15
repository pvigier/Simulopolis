#pragma once

#include <SFML/Graphics.hpp>
#include "resource/ResourceManager.h"
#include "ai/Kinematic.h"
#include "ai/SteeringBehaviors.h"
#include "render/sprite_intersection.h"

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
    Person* getOwner();
    const Person* getOwner() const;
    void setOwner(Person* owner);

private:
    static TextureManager* sTextureManager;
    static ImageManager* sImageManager;

    int mWidth;
    int mHeight;
    Kinematic mKinematic;
    SteeringBehaviors mSteering;
    sf::Sprite mSprite;
    const sf::Image& mMask;
    Person* mOwner;
};
