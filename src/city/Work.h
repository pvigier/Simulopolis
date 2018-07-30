#pragma once

#include <string>

class Person;
class Company;
class Building;

class Work
{
public:
    enum class Type{FARMER, WORKER, CRAFTSPERSON, GROCER, CASHIER, SALESPERSON, MANAGER, DOCTOR, TEACHER, POLICE_OFFICER};
    enum class Qualification{NON_QUALIFIED, QUALIFIED, HIGHLY_QUALIFIED};

    static std::string typeToString(Type type);

    Work(Type type, Building* workplace);

    Type getType() const;
    const Person* getEmployee() const;
    void setEmployee(Person* employee);
    const Company* getEmployer() const;
    void setEmployer(Company* employer);
    const Building* getWorkplace() const;
    void setSalary(float salary);
    float getSalary() const;
    float getNbHoursPerWeek() const;
    bool hasAlreadyWorkedThisMonth() const;
    void setAlreadyWorkedThisMonth(bool alreadyWorkedThisMonth);

private:
    Type mType;
    Person* mEmployee;
    Company* mEmployer;
    Building* mWorkplace;
    float mSalary;
    float mNbHoursPerWeek;
    bool mAlreadyWorkedThisMonth;
};
