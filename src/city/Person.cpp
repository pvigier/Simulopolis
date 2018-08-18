#include "city/Person.h"
#include "city/Business.h"
#include "city/Lease.h"
#include "city/City.h"
#include "message/MessageBus.h"
#include "ai/GoalRestEvaluator.h"
#include "ai/GoalWorkEvaluator.h"
#include "ai/GoalShopEvaluator.h"
#include "ai/GoalEnterCityEvaluator.h"
#include "ai/GoalGetBetterWorkEvaluator.h"

Person::Event::Event(Type type, Lease& lease) : type(type), lease(lease)
{

}

Person::Event::Event(Type type, Work& work) : type(type), work(work)
{

}

MessageBus* Person::sMessageBus = nullptr;

void Person::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

Person::Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car) :
    mId(UNDEFINED), mFirstName(firstName), mLastName(lastName), mGender(gender), mBirth(birth), mCity(nullptr),
    mState(State::WAITING), mHome(nullptr), mWork(nullptr), mFavoriteShop(nullptr), mCar(car), mMoney(0.0),
    mEnergyDecayRate(0.1f), mSatietyDecayRate(0.1f), mHealthDecayRate(0.01f), mSafetyDecayRate(0.01f), mHappinessDecayRate(0.01f),
    mEnergy(1.0f), mSatiety(1.0f), mHealth(1.0f), mSafety(1.0f), mHappiness(0.0f),
    mQualification(Work::Qualification::NON_QUALIFIED), mShortTermBrain(this), mLongTermBrain(this)
{
    mCar.setDriver(this);
    sMessageBus->addMailbox(mMailbox);

    // Add evaluators to the brains
    mShortTermBrain.addEvaluator(new GoalRestEvaluator(1.0f));
    mShortTermBrain.addEvaluator(new GoalWorkEvaluator(1.0f));
    mShortTermBrain.addEvaluator(new GoalShopEvaluator(1.0f));
    mLongTermBrain.addEvaluator(new GoalEnterCityEvaluator(1.0f));
    mLongTermBrain.addEvaluator(new GoalGetBetterWorkEvaluator(1.0f));
}

void Person::update(float dt)
{
    // AI
    mShortTermBrain.process();

    // Messages
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        mShortTermBrain.handle(message);
        mLongTermBrain.handle(message);
        if (message.type == MessageType::CITY)
        {
            const City::Event& event = message.getInfo<City::Event>();
            switch (event.type)
            {
                case City::Event::Type::NEW_MONTH:
                    mLongTermBrain.process();
                    break;
                default:
                    break;
            }
        }
    }

    // Update the car if necessary
    if (mState == State::MOVING)
        mCar.update(dt);

    // Update needs
    updateNeeds(dt);
}

MessageBus* Person::getMessageBus()
{
    return sMessageBus;
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

const City* Person::getCity() const
{
    return mCity;
}

void Person::setCity(const City* city)
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

void Person::setHome(Lease* home)
{
    mHome = home;
    home->setTenant(this);
}

void Person::leaveHome()
{
    if (mHome)
    {
        sMessageBus->send(Message::create(mHome->getOwner()->getMailboxId(), MessageType::PERSON, Event(Event::Type::LEAVE_HOUSING, *mHome)));
        mHome = nullptr;
    }
}

Work* Person::getWork()
{
    return mWork;
}

void Person::setWork(Work* work)
{
    mWork = work;
    work->setEmployee(this);
}

void Person::quitWork()
{
    if (mWork)
    {
        sMessageBus->send(Message::create(mWork->getEmployer()->getMailboxId(), MessageType::PERSON, Event(Event::Type::QUIT_WORK, *mWork)));
        mWork = nullptr;
    }
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

Money Person::getMoney() const
{
    return mMoney;
}

Money Person::getOutcome() const
{
    Money outcome(0.0);
    if (mHome)
        outcome -= mHome->getRent();
    if (mWork)
        outcome += mWork->getSalary();
    if (mFavoriteShop)
        outcome -= mFavoriteShop->getPrice();
    return outcome;
}

float Person::getEnergyDecayRate() const
{
    return mEnergyDecayRate;
}

float Person::getEnergy() const
{
    return mEnergy;
}

void Person::increaseEnergy(float delta)
{
    mEnergy = clamp(mEnergy + delta, 0.0f, 1.0f);
}

float Person::getSatiety() const
{
    return mSatiety;
}

void Person::increaseSatiety(float delta)
{
    mSatiety = clamp(mSatiety + delta, 0.0f, 1.0f);
}

float Person::getHealth() const
{
    return mHealth;
}

void Person::increaseHealth(float delta)
{
    mHealth = clamp(mHealth + delta, 0.0f, 1.0f);
}

float Person::getSafety() const
{
    return mSafety;
}

void Person::increaseSafety(float delta)
{
    mSafety = clamp(mSafety + delta, 0.0f, 1.0f);
}

float Person::getHappiness() const
{
    return mHappiness;
}

void Person::increaseHappiness(float delta)
{
    mHappiness = std::max(0.0f, mHappiness + delta);
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

void Person::updateNeeds(float dt)
{
    float dmonth = dt / mCity->getTimePerMonth();
    increaseEnergy(-dmonth * mEnergyDecayRate);
    increaseSatiety(-dmonth * mSatietyDecayRate);
    increaseHealth(-dmonth * mHealthDecayRate);
    increaseSafety(-dmonth * mSafetyDecayRate);
    increaseHappiness(-dmonth * mHappinessDecayRate);
}
