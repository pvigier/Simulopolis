#include "ai/GoalRest.h"
#include "city/City.h"
#include "city/Person.h"
#include "city/Housing.h"
#include "ai/GoalMoveTo.h"
#include "ai/GoalWait.h"

GoalRest::GoalRest(Person* owner) : Goal(owner), mAtHome(false)
{
    //ctor
}

GoalRest::~GoalRest()
{
    //dtor
}

void GoalRest::activate()
{
    mState = State::ACTIVE;

    // Add subgoals
    clearSubgoals();
    pushBack(new GoalMoveTo(mOwner, mOwner->getHome()->getHousing()));
}

Goal::State GoalRest::process()
{
    activateIfInactive();

    if (!hasFailed())
    {
        mState = processSubgoals();
        if (mAtHome)
        {
            // Update sleep and happiness
            float dt = mClock.restart().asSeconds();
            if (mSubgoals.size() == 1)
            {
                float dmonth = dt / mOwner->getCity()->getTimePerMonth();
                mOwner->increaseEnergy(Housing::ENERGY_GROWTH_RATE * dmonth);
                mOwner->increaseHappiness(mOwner->getHome()->getHousing()->getComfort() * dmonth);
            }
        }

        // If we just arrive at home
        if (isCompleted() && !mAtHome)
        {
            mClock.restart();
            mState = State::ACTIVE;
            mAtHome = true;
            mOwner->setState(Person::State::WAITING);
            // Compute the number of hours needed to be rested
            float nbMonths = (1.0f - mOwner->getEnergy()) / (Housing::ENERGY_GROWTH_RATE - mOwner->getEnergyDecayRate());
            nbMonths = std::max(0.1f, nbMonths);
            float nbHours =  nbMonths * City::NB_HOURS_PER_MONTH;
            // Add subgoal
            pushBack(new GoalWait(mOwner, nbHours));
        }
    }

    return mState;
}

void GoalRest::terminate()
{

}

bool GoalRest::handle(Message message)
{
    if (message.type == MessageType::PERSON)
    {
        const Person::Event& event = message.getInfo<Person::Event>();
        if (event.type == Person::Event::Type::EXPELLED)
        {
            mState = State::FAILED;
            std::cout << "GoalRest failed: " << static_cast<int>(mState) << std::endl;
        }
        return true;
    }
    return false;
}

std::string GoalRest::toString() const
{
    return "Rest";
}
