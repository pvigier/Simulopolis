#include "city/Car.h"

TextureManager* Car::sTextureManager = nullptr;
ImageManager* Car::sImageManager = nullptr;

Car::Car(const std::string& model) :
    mKinematic(1.0f, 150.0f, 800.0f), mSteering(mKinematic), mMask(sImageManager->getImage(model))
{
    const sf::Texture& texture = sTextureManager->getTexture(model);
    mWidth = texture.getSize().x / 8;
    mHeight = texture.getSize().y;

    mSprite.setTexture(texture);
    mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
    mSprite.setOrigin(sf::Vector2f(mWidth * 0.5f, mHeight * 0.5f));

    mSteering.setSeekDistance(32.0f);
    mSteering.setArriveDistance(32.0f);
}

void Car::setTextureManager(TextureManager* textureManager)
{
    sTextureManager = textureManager;
}

void Car::setImageManager(ImageManager* imageManager)
{
    sImageManager = imageManager;
}

void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite);
}

void Car::update(float dt)
{
    Vector2f steeringForce = mSteering.compute(dt);
    mKinematic.addForce(steeringForce);
    mKinematic.update(dt);

    mSprite.setPosition(mKinematic.getPosition());
    float angle = toDegrees(mKinematic.getDirection().angle());
    if (angle >= 0.0f)
        angle = 360.0f - angle;
    else
        angle = -angle;
    int i = static_cast<int>((angle + 22.5f) / 45.0f) % 8;
    mSprite.setTextureRect(sf::IntRect(i * mWidth, 0, mWidth, mHeight));
}

bool Car::intersect(const sf::Vector2f& position) const
{
    return sprite_intersect(mSprite, mMask, position);
}

Kinematic& Car::getKinematic()
{
    return mKinematic;
}

SteeringBehaviors& Car::getSteering()
{
    return mSteering;
}

sf::FloatRect Car::getBounds() const
{
    return mSprite.getGlobalBounds();
}
