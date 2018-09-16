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

#include <vector>
#include <boost/serialization/base_object.hpp>
#include "message/Mailbox.h"
#include "city/Tile.h"

class Company;

class Building : public Tile
{
public:
    static constexpr int STAIR_HEIGHT = 33;
    static constexpr int OFFSET_Y = 27;

    Building(const std::string& name, Type type, unsigned int nbStairs);
    virtual ~Building();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual bool intersect(const sf::Vector2f& position) const override;
    virtual std::unique_ptr<Tile> clone() const override;
    virtual void updateVariant(const Tile* neighbors[3][3]) override;

    virtual void update();
    virtual void tearDown();

    Id getId() const;
    void setId(Id id);
    sf::Vector2f getPosition() const;
    virtual void setPosition(sf::Vector2i coordinates, sf::Vector2f position) override;
    virtual void setOwner(Company* owner);
    Company* getOwner();
    const Company* getOwner() const;
    Id getMailboxId() const;
    virtual sf::FloatRect getBounds() const override;

protected:
    Id mId;
    unsigned int mNbStairs;
    Company* mOwner;
    Mailbox mMailbox;

    Building() = default;

private:
    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Tile>(*this);
        ar & mId & mNbStairs & mOwner & mMailbox;
    }
};
