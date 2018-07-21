#pragma once

#include <string>
#include "ai/GoalThink.h"
#include "city/Car.h"
#include "city/Work.h"

class City;
class Housing;
class Business;

class Person
{
public:
    enum class Gender{MALE = 0, FEMALE = 1};
    enum class State{RESTING, WORKING, MOVING, SHOPPING};

    Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car);

    void update(float dt);

    // Personal data
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;
    Gender getGender() const;
    int getAge(int year) const;
    City* getCity();
    void setCity(City* city);

    // State
    State getState() const;
    void setState(State state);

    // Daily life
    const Housing* getHome() const;
    const Work* getWork() const;
    const Business* getFavoriteShop() const;

    // Car
    Car& getCar();
    const Car& getCar() const;

    // Needs
    float getSleep() const;
    float getHealth() const;
    float getSafety() const;
    float getHunger() const;

    // Happiness
    float getHappiness() const;
    void increaseHappiness(float difference);

    // AI
    GoalThink& getShortTermBrain();
    GoalThink& getLongTermBrain();

private:
    // Personal data
    std::string mFirstName;
    std::string mLastName;
    Gender mGender;
    int mBirth;
    City* mCity;

    // State
    State mState;

    // Daily life
    Housing* mHome;
    std::unique_ptr<Work> mWork;
    const Business* mFavoriteShop;

    // Car
    Car mCar;

    // Economic data
    float mMoney;

    // Needs
    float mSleep;
    float mHealth;
    float mSafety;
    float mHunger;

    // Happiness
    float mHappiness;

    // AI
    GoalThink mShortTermBrain;
    GoalThink mLongTermBrain;
};
