#pragma once

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
};
