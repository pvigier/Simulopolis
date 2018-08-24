#pragma once

#include "ai/Goal.h"

class Business;

class GoalShop : public Goal
{
public:
    static constexpr int RADIUS = 16;

    GoalShop(Person* owner);
    virtual ~GoalShop();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    const Business* mSelectedShop;
    bool mGoodReserved;
};
