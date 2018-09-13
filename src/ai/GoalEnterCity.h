#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"

template<typename T> class Market;
class Lease;

class GoalEnterCity : public Goal
{
public:
    GoalEnterCity(Person* owner);
    virtual ~GoalEnterCity();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    const Market<Lease>* mMarket;
    bool mHomeFound;

    // Serialization
    friend class boost::serialization::access;

    GoalEnterCity() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar & mMarket & mHomeFound;
    }
};
