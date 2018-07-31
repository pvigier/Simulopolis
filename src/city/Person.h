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
    enum class State{RESTING, WORKING, MOVING, SHOPPING};

    static void setMessageBus(MessageBus* messageBus);

    Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car);

    void update(float dt);

    // Personal data
    Id getId() const;
    void setId(Id id);
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;
    Gender getGender() const;
    int getAge(int year) const;
    City* getCity();
    void setCity(City* city);
    Id getMailboxId() const;

    // State
    State getState() const;
    void setState(State state);

    // Daily life
    const Lease* getHome() const;
    const Work* getWork() const;
    std::string getWorkStatus() const;
    const Business* getFavoriteShop() const;

    // Car
    Car& getCar();
    const Car& getCar() const;

    // Finance
    float getMoney() const;
    float getOutcome() const;

    // Needs
    float getSleep() const;
    float getHealth() const;
    float getSafety() const;
    float getHunger() const;

    // Happiness
    float getHappiness() const;
    void increaseHappiness(float difference);

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
    City* mCity;
    Mailbox mMailbox;

    // State
    State mState;

    // Daily life
    Lease* mHome;
    Work* mWork;
    const Business* mFavoriteShop;

    // Car
    Car mCar;

    // Finance
    float mMoney;

    // Needs
    float mSleep;
    float mHealth;
    float mSafety;
    float mHunger;

    // Happiness
    float mHappiness;

    // Abilities
    Work::Qualification mQualification;

    // AI
    GoalThink mShortTermBrain;
    GoalThink mLongTermBrain;
};
