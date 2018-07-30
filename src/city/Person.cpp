#include "city/Person.h"
#include "city/Business.h"
#include "city/Lease.h"
#include "message/MessageBus.h"

MessageBus* Person::sMessageBus = nullptr;

Person::Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car) :
    mId(UNDEFINED), mFirstName(firstName), mLastName(lastName), mGender(gender), mBirth(birth), mCity(nullptr),
    mState(State::RESTING), mHome(nullptr), mWork(nullptr), mFavoriteShop(nullptr), mCar(car),
    mMoney(0.0f), mSleep(1.0f), mHealth(1.0f), mSafety(1.0f), mHunger(1.0f), mHappiness(0.0f),
    mQualification(Work::Qualification::NON_QUALIFIED), mShortTermBrain(this), mLongTermBrain(this)
{
    mCar.setDriver(this);
}

void Person::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

void Person::update(float dt)
{
    // AI
    mShortTermBrain.process();

    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        mShortTermBrain.handle(message);
        mLongTermBrain.handle(message);
    }

    // Update the car if necessary
    if (mState == State::MOVING)
        mCar.update(dt);
}

Id Person::getId() const
{
    return mId;
}

void Person::setId(Id id)
{
    mId = id;
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

Id Person::getMailboxId() const
{
    return mMailbox.getId();
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
    return mHome;
}

const Work* Person::getWork() const
{
    return mWork;
}

std::string Person::getWorkStatus() const
{
    if (mWork)
        return Work::typeToString(mWork->getType());
    else
        return "Unemployed";
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

Work::Qualification Person::getQualification() const
{
    return mQualification;
}

GoalThink& Person::getShortTermBrain()
{
    return mShortTermBrain;
}

const GoalThink& Person::getShortTermBrain() const
{
    return mShortTermBrain;
}

GoalThink& Person::getLongTermBrain()
{
    return mLongTermBrain;
}

const GoalThink& Person::getLongTermBrain() const
{
    return mLongTermBrain;
}
