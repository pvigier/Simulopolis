#pragma once

#include "city/Tile.h"

class CallForBids : public Tile
{
public:
    CallForBids(const std::string& name, Type type, const sf::Color& signColor);
    virtual ~CallForBids();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setPosition(sf::Vector2f position) override;
    virtual sf::FloatRect getBounds() const override;

protected:
    sf::Sprite mSignSprite;
};
