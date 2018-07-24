#pragma once

class Person;
class Company;
class Building;

class Work
{
public:
    enum class Type{FARMER, WORKER, CRAFTSMAN, GROCER, CASHIER, SELLER, MANAGER, DOCTOR, TEACHER, POLICEMAN};
    enum class Qualification{NON_QUALIFIED, QUALIFIED, HIGHLY_QUALIFIED};

    Work(Type type, const Person* employee, const Company* employer, const Building* workplace, float salary);

    Type getType() const;
    const Person* getEmployee() const;
    const Company* getEmployer() const;
    const Building* getWorkplace() const;
    float getSalary() const;
    float getNbHoursPerWeek() const;
    bool hasAlreadyWorkedThisMonth() const;
    void setAlreadyWorkedThisMonth(bool alreadyWorkedThisMonth);

private:
    Type mType;
    const Person* mEmployee;
    const Company* mEmployer;
    const Building* mWorkplace;
    float mSalary;
    float mNbHoursPerWeek;
    bool mAlreadyWorkedThisMonth;
};
