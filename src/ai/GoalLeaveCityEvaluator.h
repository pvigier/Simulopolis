#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/GoalEvaluator.h"

class GoalLeaveCityEvaluator : public GoalEvaluator
{
public:
    GoalLeaveCityEvaluator(float bias = 1.0f);

    virtual float computeDesirability(Person* person) override;

    virtual void setGoal(Person* person) override;

private:
    // Serialization
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<GoalEvaluator>(*this);
    }
};