/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "message/Mailbox.h"
#include "ai/GoalThink.h"
#include "city/Car.h"
#include "city/Work.h"
#include "city/Good.h"

class MessageBus;
class City;
class Lease;
class Business;

class Person
{
public:
    enum class Gender{MALE = 0, FEMALE = 1};
    enum class State{INVISIBLE, VISIBLE};
    enum class Need : int {ENERGY = 0, SATIETY, HEALTH, SAFETY, HAPPINESS, COUNT};

    struct Event
    {
        enum class Type{LEAVE_HOUSING, QUIT_WORK, FIRED, EXPELLED};

        Type type;
        union
        {
            Lease* lease;
            Work* work;
        };

        Event();
        Event(Type type);
        Event(Type type, Lease* lease);
        Event(Type type, Work* work);
    };

    static constexpr unsigned int NB_EVALUATORS = 6;

    Person(const std::string& firstName, const std::string& lastName, Gender gender, int birth,
        const std::string& car, const std::array<float, static_cast<int>(Need::COUNT)>& decayRates,
        double productivity, const std::array<float, NB_EVALUATORS>& biases);
    ~Person();

    void update(float dt);

    // Personal data
    Id getId() const;
    void setId(Id id);
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;
    Gender getGender() const;
    int getAge(int year) const;
    const City* getCity() const;
    void setCity(const City* city, MessageBus* messageBus, bool alreadyAdded = false);
    MessageBus* getMessageBus();
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
    Good::Type getConsumptionHabit() const;

    // Car
    Car& getCar();
    const Car& getCar() const;

    // Finance
    Id getAccount() const;
    Money getAccountBalance() const;
    Money getLastMonthOutcome() const;

    // Physiology
    float getDecayRate(Need need) const;

    // Needs
    float getNeed(Need need) const;
    void increaseNeed(Need need, float delta);
    float getAverageNeed(Need need) const;

    // Abilities
    double getProductivity() const;
    Work::Qualification getQualification() const;

    // AI
    GoalThink& getShortTermBrain();
    const GoalThink& getShortTermBrain() const;
    GoalThink& getLongTermBrain();
    const GoalThink& getLongTermBrain() const;

private:
    // Personal data
    Id mId;
    std::string mFirstName;
    std::string mLastName;
    Gender mGender;
    int mBirth;
    const City* mCity;
    MessageBus* mMessageBus;
    Mailbox mMailbox;

    // State
    State mState;

    // Daily life
    Lease* mHome;
    Work* mWork;
    Good::Type mConsumptionHabit;

    // Car
    Car mCar;

    // Finance
    Id mAccount;
    Money mLastMonthBalance;
    Money mMonthBalance;

    // Physiology
    std::array<float, static_cast<int>(Need::COUNT)> mDecayRates;

    // Needs (Physiological and security)
    std::array<float, static_cast<int>(Need::COUNT)> mNeeds;
    std::array<float, static_cast<int>(Need::COUNT)> mAverageNeeds;

    // Abilities
    float mProductivity;
    Work::Qualification mQualification;

    // AI
    GoalThink mShortTermBrain;
    GoalThink mLongTermBrain;

    void updateNeeds(float dt);

    // Events
    void onNewMonth();

    // Serialization
    friend class boost::serialization::access;

    Person() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mId & mFirstName & mLastName & mGender & mBirth & mMailbox;
        ar & mState;
        ar & mHome & mWork & mConsumptionHabit;
        ar & mCar;
        ar & mAccount & mLastMonthBalance & mMonthBalance;
        ar & mDecayRates & mNeeds & mAverageNeeds;
        ar & mProductivity & mQualification;
        ar & mShortTermBrain & mLongTermBrain;
    }
};
