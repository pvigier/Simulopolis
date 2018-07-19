#pragma once

#include <string>
#include "city/Car.h"
#include "city/Work.h"

class City;
class Housing;

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
    City* getCity();
    void setCity(City* city);
    State getState() const;
    void setState(State state);
    const Housing* getHome() const;
    const Work* getWork() const;
    Car& getCar();
    const Car& getCar() const;
    float getSleep() const;
    float getHygiene() const;
    float getSafety() const;
    float getHunger() const;
    float getHappiness() const;
    void increaseHappiness(float difference);

private:
    // Personal data
    std::string mFirstName;
    std::string mLastName;
    Gender mGender;
    int mBirth;
    City* mCity;

    // State
    State mState;

    // Home
    Housing* mHome;

    // Work
    std::unique_ptr<Work> mWork;

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
