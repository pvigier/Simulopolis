#include "CallForBids.h"
#include "resource/TextureManager.h"

CallForBids::CallForBids(const std::string& name, Type type) :
    Tile(name, type, Category::BUILDING), mSignSprite(sTextureManager->getTexture("sign"))
{
    switch (type)
    {
        case Type::CFB_HOUSING:
            mSignSprite.setColor(sf::Color(167, 125, 83));
            break;
        case Type::CFB_INDUSTRY:
            mSignSprite.setColor(sf::Color(192, 188, 170));
            break;
        case Type::CFB_BUSINESS:
            mSignSprite.setColor(sf::Color(178, 83, 83));
            break;
        case Type::CFB_HOSPITAL:
            mSignSprite.setColor(sf::Color(255, 255, 255));
            break;
        case Type::CFB_POLICE_STATION:
            mSignSprite.setColor(sf::Color(49, 122, 187));
            break;
        case Type::CFB_SCHOOL:
            mSignSprite.setColor(sf::Color(207, 156, 104));
            break;
        default:
            break;
    }
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
