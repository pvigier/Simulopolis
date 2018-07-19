#include "city/Work.h"

Work::Work(Type type, const Person* employee, const Company* employer, const Building* workplace, float salary) :
    mType(type), mEmployee(employee), mEmployer(employer), mWorkplace(workplace), mSalary(salary)
{

}

Work::Type Work::getType() const
{
    return mType;
}

const Person* Work::getEmployee() const
{
    return mEmployee;
}

const Company* Work::getEmployer() const
{
    return mEmployer;
}

const Building* Work::getWorkplace() const
{
    return mWorkplace;
}

float Work::getSalary() const
{
    return mSalary;
}

float Work::getNbHoursPerWeek() const
{
    return mNbHoursPerWeek;
}
