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
    // Add subgoals
    clearSubgoals();
    const Work* work = mOwner->getWork();
    pushBack(new GoalMoveTo(mOwner, work->getWorkplace()));
    pushBack(new GoalWait(mOwner, mOwner->getCity()->computeNbHoursInAmonth(mOwner->getCity()->getWeeklyStandardWorkingHours())));
}

Goal::State GoalWork::process()
{
    activateIfInactive();

    if (!hasFailed())
        mState = processSubgoals();

    return mState;
}

void GoalWork::terminate()
{
    mOwner->getWork()->setWorkedThisMonth(true);
}

bool GoalWork::handle(Message message)
{
    if (message.type == MessageType::PERSON)
    {
        const Person::Event& event = message.getInfo<Person::Event>();
        if (event.type == Person::Event::Type::FIRED)
            mState = State::FAILED;
        return true;
    }
    return false;
}

std::string GoalWork::toString() const
{
    return "Work";
}
