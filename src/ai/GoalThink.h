#pragma once

#include <vector>
#include "ai/Goal.h"
#include "ai/GoalEvaluator.h"

class GoalThink : public Goal
{
public:
    GoalThink(Person* owner);
    virtual ~GoalThink();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

private:
    std::vector<std::unique_ptr<GoalEvaluator>> mEvaluators;

    void arbitrate();
};
