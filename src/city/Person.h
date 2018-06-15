#pragma once

#include <string>
#include "city/Car.h"

class Person
{
public:
    enum class Gender{MALE = 0, FEMALE = 1};
    enum class State{RESTING, WORKING, MOVING, SHOPPING};

    Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth, const std::string& car);

    void update(float dt);

    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;
    Gender getGender() const;
    int getAge(int year) const;
    State getState() const;
    void setState(State state);
    Car& getCar();
    const Car& getCar() const;
    float getSleep() const;
    float getHygiene() const;
    float getSafety() const;
    float getHunger() const;
    float getHappiness() const;

private:
    // Personal data
    std::string mFirstName;
    std::string mLastName;
    Gender mGender;
    int mBirth;

    // State
    State mState;

    // Car
    Car mCar;

    // Economic data
    float mMoney;

    // Needs
    float mSleep;
    float mHygiene;
    float mSafety;
    float mHunger;

    // Happiness
    float mHappiness;
};
