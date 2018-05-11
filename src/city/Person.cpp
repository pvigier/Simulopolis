#include "Person.h"

Person::Person(const std::string& firstName, const std::string& lastName, Gender gender) :
    mFirstName(firstName), mLastName(lastName), mGender(gender), mState(State::RESTING),
    mMoney(0.0f), mSleep(1.0f), mHygiene(1.0f), mSafety(1.0f), mHunger(1.0f), mHappiness(0.0f)
{
    //ctor
}

const std::string& Person::getFirstName() const
{
    return mFirstName;
}

const std::string& Person::getLastName() const
{
    return mLastName;
}

Person::Gender Person::getGender() const
{
    return mGender;
}

int Person::getAge(int year) const
{
    return year - mBirth;
}

Person::State Person::getState() const
{
    return mState;
}

float Person::getSleep() const
{
    return mSleep;
}

float Person::getHygiene() const
{
    return mHygiene;
}

float Person::getSafety() const
{
    return mSafety;
}

float Person::getHunger() const
{
    return mHunger;
}

float Person::getHappiness() const
{
    return mHappiness;
}

