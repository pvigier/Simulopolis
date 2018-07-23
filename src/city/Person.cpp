#include "city/Person.h"
#include "city/Business.h"

Person::Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car) :
    mFirstName(firstName), mLastName(lastName), mGender(gender), mBirth(birth), mCity(nullptr),
    mState(State::RESTING), mHome(nullptr), mFavoriteShop(nullptr), mCar(car),
    mMoney(0.0f), mSleep(1.0f), mHealth(1.0f), mSafety(1.0f), mHunger(1.0f), mHappiness(0.0f),
    mShortTermBrain(this), mLongTermBrain(this)
{
    mCar.setDriver(this);
}

void Person::update(float dt)
{
    // AI
    mShortTermBrain.process();

    // Update the car if necessary
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

City* Person::getCity()
{
    return mCity;
}

void Person::setCity(City* city)
{
    mCity = city;
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

const Lease* Person::getHome() const
{
    return mHome.get();
}

const Work* Person::getWork() const
{
    return mWork.get();
}

const Business* Person::getFavoriteShop() const
{
    return mFavoriteShop;
}

Car& Person::getCar()
{
    return mCar;
}

const Car& Person::getCar() const
{
    return mCar;
}

float Person::getMoney() const
{
    return mMoney;
}

float Person::getOutcome() const
{
    float outcome = 0.0f;
    if (mHome)
        outcome -= mHome->getRent();
    if (mWork)
        outcome += mWork->getSalary();
    if (mFavoriteShop)
        outcome -= mFavoriteShop->getPrice();
    return outcome;
}

float Person::getSleep() const
{
    return mSleep;
}

float Person::getHealth() const
{
    return mHealth;
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

void Person::increaseHappiness(float difference)
{
    mHappiness += difference;
}

GoalThink& Person::getShortTermBrain()
{
    return mShortTermBrain;
}

GoalThink& Person::getLongTermBrain()
{
    return mLongTermBrain;
}
