#include "city/Work.h"

std::string Work::typeToString(Type type)
{
    switch (type)
    {
        case Type::FARMER:
            return "Farmer";
        case Type::WORKER:
            return "Worker";
        case Type::CRAFTSMAN:
            return "Craftsman";
        case Type::GROCER:
            return "Grocer";
        case Type::CASHIER:
            return "Cashier";
        case Type::SELLER:
            return "Seller";
        case Type::MANAGER:
            return "Manager";
        case Type::DOCTOR:
            return "Doctor";
        case Type::TEACHER:
            return "Teacher";
        case Type::POLICEMAN:
            return "Policeman";
        default:
            return "";
    }
}

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

bool Work::hasAlreadyWorkedThisMonth() const
{
    return mAlreadyWorkedThisMonth;
}
void Work::setAlreadyWorkedThisMonth(bool alreadyWorkedThisMonth)
{
    mAlreadyWorkedThisMonth = alreadyWorkedThisMonth;
}
