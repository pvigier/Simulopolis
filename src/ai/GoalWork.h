#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"

class GoalWork : public Goal
{
public:
    GoalWork(Person* owner);
    virtual ~GoalWork();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    // Serialization
    friend class boost::serialization::access;

    GoalWork() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
    }
};
