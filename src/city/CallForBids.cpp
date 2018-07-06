#include "CallForBids.h"
#include "resource/TextureManager.h"

CallForBids::CallForBids(const std::string& name, Type type, const sf::Color& signColor) :
    Tile(name, type, Category::BUILDING), mSignSprite(sTextureManager->getTexture("sign"))
{
    mSignSprite.setColor(signColor);
}

CallForBids::~CallForBids()
{
    //dtor
}

void CallForBids::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite);
    // Sign
    target.draw(mSignSprite);
}

std::unique_ptr<Tile> CallForBids::clone() const
{
    return std::unique_ptr<Tile>(new CallForBids(*this));
}

void CallForBids::setPosition(sf::Vector2f position)
{
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
