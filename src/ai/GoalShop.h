#pragma once

#include "ai/Goal.h"

class GoalShop : public Goal
{
public:
    GoalShop(Person* owner);
    virtual ~GoalShop();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
};
