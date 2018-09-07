#pragma once

#include "ai/Goal.h"

class GoalLeaveCity : public Goal
{
public:
    GoalLeaveCity(Person* owner);
    virtual ~GoalLeaveCity();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
};
