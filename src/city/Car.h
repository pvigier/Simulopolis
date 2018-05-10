#pragma once

#include <SFML/Graphics.hpp>
#include "resource/ResourceManager.h"
#include "ai/Kinematic.h"
#include "ai/SteeringBehaviors.h"

class Car : public sf::Drawable
{
public:
    static constexpr unsigned int WIDTH = 33;
    static constexpr unsigned int HEIGHT = 27;

    Car();

    static void setTextureManager(TextureManager* textureManager);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);

    Kinematic& getKinematic();
    SteeringBehaviors& getSteering();

    sf::Vector2f getBottomRight() const;

private:
    static TextureManager* sTextureManager;

    Kinematic mKinematic;
    SteeringBehaviors mSteering;
    sf::Sprite mSprite;
};
