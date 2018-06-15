#include "Person.h"

Person::Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car) :
    mFirstName(firstName), mLastName(lastName), mGender(gender), mBirth(birth), mState(State::RESTING),
    mCar(car), mMoney(0.0f), mSleep(1.0f), mHygiene(1.0f), mSafety(1.0f), mHunger(1.0f),
    mHappiness(0.0f)
{
    mCar.setOwner(this);
}

void Person::update(float dt)
{
    if (mState == State::MOVING)
        mCar.update(dt);
}

const std::string& Person::getFirstName() const
{
    return mFirstName;
}

const std::string& Person::getLastName() const
{
    return mLastName;
}

std::string Person::getFullName() const
{
    return mFirstName + " " + mLastName;
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

void Person::setState(Person::State state)
{
    mState = state;
}

Car& Person::getCar()
{
    return mCar;
}

const Car& Person::getCar() const
{
    return mCar;
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

