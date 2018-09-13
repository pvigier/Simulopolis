#include "city/Building.h"
#include "message/MessageBus.h"
#include "render/sprite_intersection.h"
#include "city/Person.h"
#include "city/Company.h"

Building::Building(const std::string& name, Type type, unsigned int nbStairs) :
    Tile(name, type, Category::BUILDING), mId(UNDEFINED), mNbStairs(nbStairs), mOwner(nullptr)
{

}

Building::~Building()
{
    // Unregister mailbox
    if (mMailbox.getId() != UNDEFINED)
        mOwner->getMessageBus()->removeMailbox(mMailbox);
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    // Stairs
    sf::Sprite sprite(mSprite);
    sprite.setTextureRect(sf::IntRect(mSprite.getTextureRect().left, 0, 132, 85));
    for (unsigned int i = 0; i < mNbStairs - 1; ++i)
    {
        sprite.move(0, -STAIR_HEIGHT);
        target.draw(sprite, states);
    }
}

bool Building::intersect(const sf::Vector2f& position) const
{
    if (sprite_intersect(mSprite, *mMask, position))
        return true;
    // Stairs
    sf::Sprite sprite(mSprite);
    sprite.setTextureRect(sf::IntRect(mSprite.getTextureRect().left, 0, 132, 85));
    for (unsigned int i = 0; i < mNbStairs - 1; ++i)
    {
        sprite.move(0, -STAIR_HEIGHT);
        if (sprite_intersect(sprite, *mMask, position))
            return true;
    }
    return false;
}

std::unique_ptr<Tile> Building::clone() const
{
    return std::make_unique<Building>(mTextureName, mType, mNbStairs);
}

void Building::updateVariant(const Tile* neighbors[3][3])
{
    sf::IntRect rect(0, 85, 132, 128);

    if (neighbors[2][1]->isRoad())
        rect.left = 0;
    else if (neighbors[1][2]->isRoad())
        rect.left = 132;
    else if (neighbors[0][1]->isRoad())
        rect.left = 264;
    else
        rect.left = 396;

    mSprite.setTextureRect(rect);
}

void Building::update()
{
    while (!mMailbox.isEmpty())
        mMailbox.get();
}

void Building::tearDown()
{

}

Id Building::getId() const
{
    return mId;
}

void Building::setId(Id id)
{
    mId = id;
}

sf::Vector2f Building::getPosition() const
{
    return mSprite.getPosition();
}

void Building::setPosition(sf::Vector2i coordinates, sf::Vector2f position)
{
    mCoordinates = coordinates;
    mSprite.setPosition(sf::Vector2f(position.x, position.y - OFFSET_Y));
}

Company* Building::getOwner()
{
    return mOwner;
}

const Company* Building::getOwner() const
{
    return mOwner;
}

void Building::setOwner(Company* owner)
{
    // Unregister mailbox
    if (mMailbox.getId() != UNDEFINED)
        mOwner->getMessageBus()->removeMailbox(mMailbox);
    // Update owner
    mOwner = owner;
    // Register mailbox
    if (owner != nullptr)
        mOwner->getMessageBus()->addMailbox(mMailbox);
}

Id Building::getMailboxId() const
{
    return mMailbox.getId();
}

sf::FloatRect Building::getBounds() const
{
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    bounds.top -= (mNbStairs - 1) * STAIR_HEIGHT;
    bounds.height += (mNbStairs - 1) * STAIR_HEIGHT;
    return bounds;
}
