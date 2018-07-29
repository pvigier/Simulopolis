#pragma once

#include "ai/Goal.h"

class GoalRest : public Goal
{
public:
    GoalRest(Person* owner);
    virtual ~GoalRest();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
};
