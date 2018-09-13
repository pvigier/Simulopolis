#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"

class GoalWait : public Goal
{
public:
    GoalWait(Person* owner, float nbHours);
    virtual ~GoalWait();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
    float mDuration;
    float mEndTime;

    // Serialization
    friend class boost::serialization::access;

    GoalWait() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar & mDuration & mEndTime;
    }
};
