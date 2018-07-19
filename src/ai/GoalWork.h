#pragma once

#include "ai/Goal.h"

class GoalWork : public Goal
{
public:
    GoalWork(Person* owner);
    virtual ~GoalWork();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

private:
};
