#include "city/Person.h"
#include "city/Business.h"
#include "city/Lease.h"
#include "city/City.h"
#include "message/MessageBus.h"
#include "ai/GoalRestEvaluator.h"
#include "ai/GoalWorkEvaluator.h"
#include "ai/GoalShopEvaluator.h"
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
    mState(State::WAITING), mHome(nullptr), mWork(nullptr), mFavoriteShop(nullptr), mCar(car),
    mMoney(0.0f), mSleep(1.0f), mHealth(1.0f), mSafety(1.0f), mHunger(1.0f), mHappiness(0.0f),
    mQualification(Work::Qualification::NON_QUALIFIED), mShortTermBrain(this), mLongTermBrain(this)
{
    mCar.setDriver(this);
    sMessageBus->addMailbox(mMailbox);

    // Add evaluators to the brains
    mShortTermBrain.addEvaluator(new GoalRestEvaluator(1.0f));
    mShortTermBrain.addEvaluator(new GoalWorkEvaluator(1.0f));
    mShortTermBrain.addEvaluator(new GoalShopEvaluator(1.0f));
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

const Work* Person::getWork() const
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

void Person::increaseSleep(float delta)
{
    mSleep = clamp(mSleep + delta, 0.0f, 1.0f);
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

float Person::getHunger() const
{
    return mHunger;
}

void Person::increaseHunger(float delta)
{
    mHunger = clamp(mHunger + delta, 0.0f, 1.0f);
}

float Person::getHappiness() const
{
    return mHappiness;
}

void Person::increaseHappiness(float delta)
{
    mHappiness = std::min(0.0f, mHappiness + delta);
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
    static float sum = 0.0f;
    sum += dt;
    float dmonth = dt / 10.0f;
    increaseSleep(-dmonth * 0.1f);
    increaseHealth(-dmonth * 0.01f);
    increaseSafety(-dmonth * 0.01f);
    increaseHunger(-dmonth * 0.1f);
}
