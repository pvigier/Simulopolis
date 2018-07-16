#include "city/Work.h"

Work::Work(Type type, Person* person, float salary) : mType(type), mPerson(person), mSalary(salary)
{

}

Work::Type Work::getType() const
{
    return mType;
}

const Person* Work::getPerson() const
{
    return mPerson;
}

float Work::getSalary() const
{
    return mSalary;
}
