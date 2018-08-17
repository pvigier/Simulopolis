#include "city/Work.h"
#include "city/Person.h"

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
    mType(type), mEmployee(nullptr), mEmployer(nullptr), mWorkplace(workplace), mSalary(0.0)
{
    switch (mType)
    {
        case Type::FARMER:
        case Type::GROCER:
        case Type::CASHIER:
        case Type::WORKER:
            mQualification = Qualification::NON_QUALIFIED;
            break;
        case Type::CRAFTSPERSON:
        case Type::SALESPERSON:
        case Type::TEACHER:
        case Type::POLICE_OFFICER:
            mQualification = Qualification::QUALIFIED;
            break;
        case Type::MANAGER:
        case Type::DOCTOR:
            mQualification = Qualification::HIGHLY_QUALIFIED;
            break;
        default:
            break;
    }
}

Work::Type Work::getType() const
{
    return mType;
}

Work::Qualification Work::getQualification() const
{
    return mQualification;
}

const Person* Work::getEmployee() const
{
    return mEmployee;
}

std::string Work::getEmployeeName() const
{
    if (mEmployee)
        return mEmployee->getFullName();
    else
        return "Vacant";
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

void Work::setSalary(Money salary)
{
    mSalary = salary;
}

Money Work::getSalary() const
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
