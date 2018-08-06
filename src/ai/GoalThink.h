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

    void addEvaluator(GoalEvaluator* evaluator);

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    std::vector<std::unique_ptr<GoalEvaluator>> mEvaluators;

    void arbitrate();
};
