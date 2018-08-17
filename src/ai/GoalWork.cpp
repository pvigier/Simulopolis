#include "GoalWork.h"
#include "city/Person.h"
#include "city/Work.h"
#include "city/Building.h"
#include "city/City.h"
#include "GoalMoveTo.h"
#include "GoalWait.h"

GoalWork::GoalWork(Person* owner) : Goal(owner)
{
    //ctor
}

GoalWork::~GoalWork()
{
    //dtor
}

void GoalWork::activate()
{
    mState = State::ACTIVE;
    mOwner->getWork()->setAlreadyWorkedThisMonth(true);
    // Add subgoals
    clearSubgoals();
    const Work* work = mOwner->getWork();
    pushBack(new GoalMoveTo(mOwner, work->getWorkplace()));
    pushBack(new GoalWait(mOwner, mOwner->getCity()->computeNbHoursInAmonth(mOwner->getCity()->getWeeklyStandardWorkingHours())));
}

Goal::State GoalWork::process()
{
    activateIfInactive();

    mState = processSubgoals();
    return mState;
}

void GoalWork::terminate()
{

}

std::string GoalWork::toString() const
{
    return "Work";
}
