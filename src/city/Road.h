#pragma once

#include <boost/serialization/base_object.hpp>
#include "city/Tile.h"

class Road : public Tile
{
public:
    Road(const std::string& name, Type type);
    virtual ~Road();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void updateVariant(const Tile* neighbors[3][3]) override;

private:
    // Serialization
    friend class boost::serialization::access;

    Road() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Tile>(*this);
    }
};
