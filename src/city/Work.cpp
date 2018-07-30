#include "city/Work.h"

std::string Work::typeToString(Type type)
{
    switch (type)
    {
        case Type::FARMER:
            return "Farmer";
        case Type::WORKER:
            return "Worker";
        case Type::CRAFTSPERSON:
            return "Craftsperson";
        case Type::GROCER:
            return "Grocer";
        case Type::CASHIER:
            return "Cashier";
        case Type::SALESPERSON:
            return "Salesperson";
        case Type::MANAGER:
            return "Manager";
        case Type::DOCTOR:
            return "Doctor";
        case Type::TEACHER:
            return "Teacher";
        case Type::POLICE_OFFICER:
            return "Police officer";
        default:
            return "";
    }
}

Work::Work(Type type, Building* workplace) :
    mType(type), mEmployee(nullptr), mEmployer(nullptr), mWorkplace(workplace), mSalary(0.0f)
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

void Work::setEmployee(Person* employee)
{
    mEmployee = employee;
}

const Company* Work::getEmployer() const
{
    return mEmployer;
}

void Work::setEmployer(Company* employer)
{
    mEmployer = employer;
}

const Building* Work::getWorkplace() const
{
    return mWorkplace;
}

void Work::setSalary(float salary)
{
    mSalary = salary;
}

float Work::getSalary() const
{
    return mSalary;
}

float Work::getNbHoursPerWeek() const
{
    return mNbHoursPerWeek;
}

bool Work::hasAlreadyWorkedThisMonth() const
{
    return mAlreadyWorkedThisMonth;
}
void Work::setAlreadyWorkedThisMonth(bool alreadyWorkedThisMonth)
{
    mAlreadyWorkedThisMonth = alreadyWorkedThisMonth;
}
