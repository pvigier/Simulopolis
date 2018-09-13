#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"

class GoalLeaveCity : public Goal
{
public:
    GoalLeaveCity(Person* owner);
    virtual ~GoalLeaveCity();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
    // Serialization
    friend class boost::serialization::access;

    GoalLeaveCity() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
    }
};
