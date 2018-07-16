#pragma once

class Person;

class Work
{
    enum class Type{FARMER, WORKER, CRAFTSMAN, GROCER, CASHIER, SELLER, MANAGER, DOCTOR, TEACHER, POLICEMAN};

    Work(Type type, Person* person, float salary);

    Type getType() const;
    const Person* getPerson() const;
    float getSalary() const;

private:
    Type mType;
    const Person* mPerson;
    float mSalary;
};
