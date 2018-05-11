#pragma once

class Person
{
public:
    enum class Gender{MALE, FEMALE};
    enum class State{RESTING, WORKING, MOVING, SHOPPING}

    Person(const std::string& firstName, const std::string& lastName, Gender gender);

    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    Gender getGender() const;
    int getAge(int year) const;
    State getState() const;
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
