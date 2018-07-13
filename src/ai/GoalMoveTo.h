#pragma once

#include "ai/Goal.h"
#include "ai/Path.h"

class GoalMoveTo : public Goal
{
public:
    GoalMoveTo(const sf::Vector2i& target);
    virtual ~GoalMoveTo();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

private:
    sf::Vector2i mTarget;
};
