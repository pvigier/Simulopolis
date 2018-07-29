#pragma once

#include "city/Market.h"
#include "city/Work.h"
#include "ai/Goal.h"

class GoalGetBetterWork : public Goal
{
public:
    GoalGetBetterWork(Person* owner, unsigned int nbMonthsBeforeFailing);
    virtual ~GoalGetBetterWork();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    unsigned int mNbMonthsBeforeFailing;
    Market<Work>* mMarket;
};
