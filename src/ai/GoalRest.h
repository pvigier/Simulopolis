#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"

class GoalRest : public Goal
{
public:
    GoalRest(Person* owner);
    virtual ~GoalRest();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    bool mAtHome;
    float mLastUpdate;

    // Serialization
    friend class boost::serialization::access;

    GoalRest() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar & mAtHome & mLastUpdate;
    }
};
