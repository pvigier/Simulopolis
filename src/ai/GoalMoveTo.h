#pragma once

#include "ai/Goal.h"
#include "ai/Path.h"

class Tile;

class GoalMoveTo : public Goal
{
public:
    GoalMoveTo(Person* owner, const Tile* target);
    virtual ~GoalMoveTo();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
    static constexpr float ARRIVE_DISTANCE = 1.0f;

    const Tile* mTarget;

    // Serialization
    friend class boost::serialization::access;

    GoalMoveTo() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        //ar & mTarget;
    }
};
