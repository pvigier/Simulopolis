#include "city/Person.h"
#include "city/Business.h"
#include "city/Lease.h"
#include "city/City.h"
#include "message/MessageBus.h"
#include "ai/GoalRestEvaluator.h"
#include "ai/GoalWorkEvaluator.h"
#include "ai/GoalShopEvaluator.h"
#include "ai/GoalEnterCityEvaluator.h"
#include "ai/GoalLeaveCityEvaluator.h"
#include "ai/GoalGetBetterHomeEvaluator.h"
#include "ai/GoalGetBetterWorkEvaluator.h"

Person::Event::Event()
{

}

Person::Event::Event(Type type) : type(type)
{

}

Person::Event::Event(Type type, Lease* lease) : type(type), lease(lease)
{

}

Person::Event::Event(Type type, Work* work) : type(type), work(work)
{

}

Person::Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car) :
    mId(UNDEFINED), mFirstName(firstName), mLastName(lastName), mGender(gender), mBirth(birth),
    mCity(nullptr), mMessageBus(nullptr),
    mState(State::WAITING), mHome(nullptr), mWork(nullptr), mConsumptionHabit(Good::NECESSARY), mCar(car),
    mAccount(UNDEFINED), mLastMonthBalance(0.0), mMonthBalance(0.0),
    mDecayRates{0.1f, 0.1f, 0.01f, 0.01f, 0.1f},
    mNeeds{1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, mAverageNeeds{0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    mQualification(Work::Qualification::NON_QUALIFIED), mShortTermBrain(this), mLongTermBrain(this)
{
    mCar.setDriver(this);

    // Add evaluators to the brains
    mShortTermBrain.addEvaluator(std::make_unique<GoalRestEvaluator>(1.0f));
    mShortTermBrain.addEvaluator(std::make_unique<GoalWorkEvaluator>(1.0f));
    mShortTermBrain.addEvaluator(std::make_unique<GoalShopEvaluator>(1.0f));
    mLongTermBrain.addEvaluator(std::make_unique<GoalEnterCityEvaluator>(1.0f));
    mLongTermBrain.addEvaluator(std::make_unique<GoalLeaveCityEvaluator>(1.0f));
    mLongTermBrain.addEvaluator(std::make_unique<GoalGetBetterHomeEvaluator>(1.0f));
    mLongTermBrain.addEvaluator(std::make_unique<GoalGetBetterWorkEvaluator>(1.0f));
}

Person::~Person()
{
    // Close bank account
    if (mAccount != UNDEFINED)
        mMessageBus->send(Message::create(mMailbox.getId(), mCity->getBank().getMailboxId(), MessageType::BANK, mCity->getBank().createCloseAccountEvent(mAccount)));
    // Unregister mailbox
    if (mMailbox.getId() != UNDEFINED)
        mMessageBus->removeMailbox(mMailbox);
}

void Person::update(float dt)
{
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
                    onNewMonth();
                    break;
                default:
                    break;
            }
        }
        else if (message.type == MessageType::BANK)
        {
            const Bank::Event& event = message.getInfo<Bank::Event>();
            switch (event.type)
            {
                case Bank::Event::Type::ACCOUNT_CREATED:
                    mAccount = event.account;
                    break;
                default:
                    break;
            }
        }
        else if (message.type == MessageType::PERSON)
        {
            const Event& event = message.getInfo<Event>();
            switch (event.type)
            {
                case Event::Type::EXPELLED:
                    mHome = nullptr;
                    break;
                case Event::Type::FIRED:
                    mWork = nullptr;
                    break;
                default:
                    break;
            }
        }
    }

    // AI
    mShortTermBrain.process();

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

const City* Person::getCity() const
{
    return mCity;
}

void Person::setCity(const City* city, MessageBus* messageBus, bool alreadyAdded)
{
    mCity = city;
    mMessageBus = messageBus;
    if (!alreadyAdded)
    {
        mMessageBus->addMailbox(mMailbox);
        // Create bank account
        mMessageBus->send(Message::create(mMailbox.getId(), mCity->getBank().getMailboxId(), MessageType::BANK, mCity->getBank().createCreateAccountEvent(Bank::Account::Type::PERSON)));
    }
}

MessageBus* Person::getMessageBus()
{
    return mMessageBus;
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
        mMessageBus->send(Message::create(mHome->getOwner()->getMailboxId(), MessageType::PERSON, Event(Event::Type::LEAVE_HOUSING, mHome)));
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
        mMessageBus->send(Message::create(mWork->getEmployer()->getMailboxId(), MessageType::PERSON, Event(Event::Type::QUIT_WORK, mWork)));
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

Good Person::getConsumptionHabit() const
{
    return mConsumptionHabit;
}

Car& Person::getCar()
{
    return mCar;
}

const Car& Person::getCar() const
{
    return mCar;
}

Id Person::getAccount() const
{
    return mAccount;
}

Money Person::getAccountBalance() const
{
    if (mAccount != UNDEFINED)
        return mCity->getBank().getBalance(mAccount);
    return Money(0.0);
}

Money Person::getLastMonthOutcome() const
{
    return Money(mMonthBalance - mLastMonthBalance);
}

float Person::getDecayRate(Need need) const
{
    return mDecayRates[static_cast<int>(need)];
}

float Person::getNeed(Need need) const
{
    return mNeeds[static_cast<int>(need)];
}

void Person::increaseNeed(Need need, float delta)
{
    mNeeds[static_cast<int>(need)] = clamp(mNeeds[static_cast<int>(need)] + delta, 0.0f, 1.0f);
}

float Person::getAverageNeed(Need need) const
{
    return mAverageNeeds[static_cast<int>(need)];
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
    for (int i = 0; i < static_cast<int>(Need::COUNT); ++i)
    {
        increaseNeed(static_cast<Need>(i), -dmonth * mDecayRates[i]);
        mAverageNeeds[i] += dmonth * mNeeds[i];
    }
}

void Person::onNewMonth()
{
    mLastMonthBalance = mMonthBalance;
    mMonthBalance = mCity->getBank().getBalance(mAccount);
    mLongTermBrain.process();
    // Reset averages
    mAverageNeeds.fill(0.0f);
}
