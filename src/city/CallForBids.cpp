#include "CallForBids.h"
#include "resource/TextureManager.h"

CallForBids::CallForBids(const std::string& name, Type type, const sf::Color& signColor) :
    Tile(name, type, Category::CALL_FOR_BIDS), mSignColor(signColor)
{
    setUpSign();
}

CallForBids::~CallForBids()
{
    //dtor
}

void CallForBids::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    // Sign
    target.draw(mSignSprite, states);
}

std::unique_ptr<Tile> CallForBids::clone() const
{
    return std::make_unique<CallForBids>(mTextureName, mType, mSignColor);
}

void CallForBids::setPosition(sf::Vector2i coordinates, sf::Vector2f position)
{
    mCoordinates = coordinates;
    mSprite.setPosition(position);
    mSignSprite.setPosition(position + sf::Vector2f(53.0f, -8.0f));
}

sf::FloatRect CallForBids::getBounds() const
{
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    bounds.top -= 8.0f;
    bounds.height += 8.0f;
    return bounds;
}

void CallForBids::setUpSign()
{
    mSignSprite.setTexture(sTextureManager->getTexture("sign"));
    mSignSprite.setColor(mSignColor);
}
