#pragma once

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"

class Business;

class GoalShop : public Goal
{
public:
    static constexpr int RADIUS = 16;

    GoalShop(Person* owner);
    virtual ~GoalShop();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    const Business* mSelectedShop;
    bool mGoodReserved;

    // Serialization
    friend class boost::serialization::access;

    GoalShop() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar /*& mSelectedShop*/ & mGoodReserved;
    }
};
