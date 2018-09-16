#pragma once

#include <string>
#include <boost/serialization/access.hpp>
#include "city/Money.h"

class Person;
class Company;
class Building;

class Work
{
public:
    enum class Type{FARMER, WORKER, CRAFTSPERSON, GROCER, CASHIER, SALESPERSON, MANAGER, DOCTOR, TEACHER, POLICE_OFFICER};
    enum class Qualification : int {NON_QUALIFIED = 0, QUALIFIED, HIGHLY_QUALIFIED};

    static std::string typeToString(Type type);

    Work();
    Work(Type type, Building* workplace);

    Type getType() const;
    Qualification getQualification() const;
    const Person* getEmployee() const;
    std::string getEmployeeName() const;
    void setEmployee(Person* employee);
    const Company* getEmployer() const;
    void setEmployer(Company* employer);
    const Building* getWorkplace() const;
    void setSalary(Money salary);
    Money getSalary() const;
    bool hasWorkedThisMonth() const;
    void setWorkedThisMonth(bool workedThisMonth);

private:
    Type mType;
    Qualification mQualification;
    Person* mEmployee;
    Company* mEmployer;
    Building* mWorkplace;
    Money mSalary;
    bool mWorkedThisMonth;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mType & mQualification & mEmployee & mEmployer & mWorkplace & mSalary & mWorkedThisMonth;
    }
};
