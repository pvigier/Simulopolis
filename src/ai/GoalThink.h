#pragma once

#include <vector>
#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"
#include "ai/GoalEvaluator.h"

class GoalThink : public Goal
{
public:
    GoalThink(Person* owner = nullptr);
    virtual ~GoalThink();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    void addEvaluator(std::unique_ptr<GoalEvaluator> evaluator);

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    std::vector<std::unique_ptr<GoalEvaluator>> mEvaluators;

    void arbitrate();

    // Serialization
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar & mEvaluators;
    }
};
