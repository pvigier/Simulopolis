#include "city/Car.h"
#include "resource/TextureManager.h"
#include "resource/ImageManager.h"

TextureManager* Car::sTextureManager = nullptr;
ImageManager* Car::sImageManager = nullptr;

void Car::setTextureManager(TextureManager* textureManager)
{
    sTextureManager = textureManager;
}

void Car::setImageManager(ImageManager* imageManager)
{
    sImageManager = imageManager;
}

Car::Car(std::string model) :
    mModel(std::move(model)), mKinematic(1.0f, 150.0f), mSteering(&mKinematic),
    mMask(nullptr), mDriver(nullptr)
{
    mSteering.setSeekDistance(4.0f);
    mSteering.setArriveDistance(4.0f);

    setUp();
}

void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Debug : draw path
    /*const Path& path = mSteering.getPath();
    std::vector<sf::Vertex> lines;
    for (const Vector2f& point : path.getPoints())
        lines.emplace_back(point);
    target.draw(lines.data(), lines.size(), sf::LinesStrip);*/
    // Draw car
    target.draw(mSprite, states);
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
    return sprite_intersect(mSprite, *mMask, position);
}

Kinematic& Car::getKinematic()
{
    return mKinematic;
}

const Kinematic& Car::getKinematic() const
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

Person* Car::getDriver()
{
    return mDriver;
}

const Person* Car::getDriver() const
{
    return mDriver;
}

void Car::setDriver(Person* owner)
{
    mDriver = owner;
}

void Car::setUp()
{
    mMask = &sImageManager->getImage(mModel);

    const sf::Texture& texture = sTextureManager->getTexture(mModel);
    mWidth = texture.getSize().x / 8;
    mHeight = texture.getSize().y;

    mSprite.setTexture(texture);
    mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
    mSprite.setOrigin(sf::Vector2f(mWidth * 0.5f, mHeight * 0.5f));
}

