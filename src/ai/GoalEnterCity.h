#pragma once

#include "city/Housing.h"
#include "ai/Goal.h"

template<typename T> class Market;

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
    Market<Lease>* mMarket;
    bool mHomeFound;
};
