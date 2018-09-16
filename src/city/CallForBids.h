#pragma once

#include <boost/serialization/base_object.hpp>
#include "city/Tile.h"

class CallForBids : public Tile
{
public:
    CallForBids(const std::string& name, Type type, const sf::Color& signColor);
    virtual ~CallForBids();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setPosition(sf::Vector2i coordinates, sf::Vector2f position) override;
    virtual sf::FloatRect getBounds() const override;

protected:
    sf::Color mSignColor;
    sf::Sprite mSignSprite;

private:
    void setUpSign();

    // Serialization
    friend class boost::serialization::access;

    CallForBids() = default;

    template<typename Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
        ar & boost::serialization::base_object<Tile>(*this);
        ar & mSignColor;
    }

    template<typename Archive>
    void load(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Tile>(*this);
        ar & mSignColor;
        setUpSign();
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};
