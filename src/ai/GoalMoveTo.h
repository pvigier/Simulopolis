#pragma once

#include "ai/Goal.h"
#include "ai/Path.h"

class GoalMoveTo : public Goal
{
public:
    GoalMoveTo(Person* owner, const sf::Vector2i& target);
    virtual ~GoalMoveTo();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
    sf::Vector2i mTarget;
};
