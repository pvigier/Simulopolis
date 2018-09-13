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

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Tile>(*this);
        ar & mId & mNbStairs & mOwner & mMailbox;
    }
};
