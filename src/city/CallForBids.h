/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <boost/serialization/base_object.hpp>
#include "city/Tile.h"

class CallForBids : public Tile
{
public:
    CallForBids(const std::string& name, Type type, const sf::Color& signColor);

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
