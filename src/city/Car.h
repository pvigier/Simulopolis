#pragma once

#include <SFML/Graphics.hpp>
#include "resource/ResourceManager.h"
#include "ai/Kinematic.h"
#include "ai/SteeringBehaviors.h"

class Car : public sf::Drawable
{
public:
    Car(const std::string& model);

    static void setTextureManager(TextureManager* textureManager);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);

    Kinematic& getKinematic();
    SteeringBehaviors& getSteering();

    sf::FloatRect getBounds() const;

private:
    static TextureManager* sTextureManager;

    int mWidth;
    int mHeight;
    Kinematic mKinematic;
    SteeringBehaviors mSteering;
    sf::Sprite mSprite;
};
