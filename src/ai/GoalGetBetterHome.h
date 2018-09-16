#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"

template <typename T> class Market;
class Lease;

class GoalGetBetterHome : public Goal
{
public:
    GoalGetBetterHome(Person* owner, unsigned int nbMonthsBeforeFailing);
    virtual ~GoalGetBetterHome();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    unsigned int mNbMonthsBeforeFailing;
    const Market<Lease>* mMarket;

    // Serialization
    friend class boost::serialization::access;

    GoalGetBetterHome() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar & mNbMonthsBeforeFailing & mMarket;
    }
};
