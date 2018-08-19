#pragma once

#include "message/Mailbox.h"
#include "ai/GoalThink.h"
#include "city/Car.h"
#include "city/Work.h"

class MessageBus;
class City;
class Lease;
class Business;

class Person
{
public:
    enum class Gender{MALE = 0, FEMALE = 1};
    enum class State{WAITING, MOVING};

    struct Event
    {
        enum class Type{LEAVE_HOUSING, QUIT_WORK};

        Type type;
        union
        {
            Lease& lease;
            Work& work;
        };

        Event(Type type, Lease& lease);
        Event(Type type, Work& work);
    };

    static void setMessageBus(MessageBus* messageBus);

    Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car);
    ~Person();

    void update(float dt);

    MessageBus* getMessageBus();

    // Personal data
    Id getId() const;
    void setId(Id id);
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;
    Gender getGender() const;
    int getAge(int year) const;
    const City* getCity() const;
    void setCity(const City* city);
    Id getMailboxId() const;

    // State
    State getState() const;
    void setState(State state);

    // Daily life
    const Lease* getHome() const;
    void setHome(Lease* home);
    void leaveHome();
    Work* getWork();
    void setWork(Work* work);
    void quitWork();
    std::string getWorkStatus() const;
    const Business* getFavoriteShop() const;

    // Car
    Car& getCar();
    const Car& getCar() const;

    // Finance
    Id getAccount() const;
    Money getAccountBalance() const;
    Money getLastMonthOutcome() const;

    // Physiology
    float getEnergyDecayRate() const;

    // Needs
    float getEnergy() const;
    void increaseEnergy(float delta);
    float getSatiety() const;
    void increaseSatiety(float delta);
    float getHealth() const;
    void increaseHealth(float delta);
    float getSafety() const;
    void increaseSafety(float delta);

    // Happiness
    float getHappiness() const;
    void increaseHappiness(float delta);

    // Abilities
    Work::Qualification getQualification() const;

    // AI
    GoalThink& getShortTermBrain();
    const GoalThink& getShortTermBrain() const;
    GoalThink& getLongTermBrain();
    const GoalThink& getLongTermBrain() const;

private:
    static MessageBus* sMessageBus;

    // Personal data
    Id mId;
    std::string mFirstName;
    std::string mLastName;
    Gender mGender;
    int mBirth;
    const City* mCity;
    Mailbox mMailbox;

    // State
    State mState;

    // Daily life
    Lease* mHome;
    Work* mWork;
    Business* mFavoriteShop;

    // Car
    Car mCar;

    // Finance
    Id mAccount;
    Money mLastMonthBalance;
    Money mMonthBalance;

    // Physiology
    float mEnergyDecayRate;
    float mSatietyDecayRate;
    float mHealthDecayRate;
    float mSafetyDecayRate;
    float mHappinessDecayRate;

    // Needs (Physiological and security)
    float mEnergy;
    float mSatiety;
    float mHealth;
    float mSafety;

    // Happiness
    float mHappiness;

    // Abilities
    Work::Qualification mQualification;

    // AI
    GoalThink mShortTermBrain;
    GoalThink mLongTermBrain;

    void updateNeeds(float dt);

    // Events
    void onNewMonth();
};
