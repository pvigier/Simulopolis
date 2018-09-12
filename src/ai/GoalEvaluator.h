#pragma once

#include <boost/serialization/access.hpp>

class Person;

class GoalEvaluator
{
public:
    GoalEvaluator(float bias);
    virtual ~GoalEvaluator();

    virtual float computeDesirability(Person* person) = 0;

    virtual void setGoal(Person* person) = 0;

protected:
    float mBias;

private:
    // Serialization
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mBias;
    }
};
