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

Work::Qualification Work::typeToQualification(Work::Type type)
{
    switch (type)
    {
        case Type::FARMER:
        case Type::GROCER:
        case Type::CASHIER:
        case Type::WORKER:
            return Qualification::NON_QUALIFIED;
        case Type::CRAFTSPERSON:
        case Type::SALESPERSON:
        case Type::TEACHER:
        case Type::POLICE_OFFICER:
            return Qualification::QUALIFIED;
        case Type::MANAGER:
        case Type::DOCTOR:
            return Qualification::HIGHLY_QUALIFIED;
        default:
            return Qualification::NON_QUALIFIED;
    }
}

float Work::typeToArduousness(Work::Type type)
{
    switch (typeToQualification(type))
    {
        case Qualification::NON_QUALIFIED:
            return 0.5f;
        case Qualification::QUALIFIED:
            return 0.25f;
        case Qualification::HIGHLY_QUALIFIED:
            return 0.0f;
        default:
            return 0.0f;
    }
}

Work::Work(Type type, Building* workplace) :
    mType(type), mQualification(typeToQualification(mType)),
    mEmployee(nullptr), mEmployer(nullptr), mWorkplace(workplace),
    mSalary(0.0), mArduousness(typeToArduousness(mType)), mWorkedThisMonth(false)
{

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

float Work::getArduousness() const
{
    return mArduousness;
}

bool Work::hasWorkedThisMonth() const
{
    return mWorkedThisMonth;
}

void Work::setWorkedThisMonth(bool workedThisMonth)
{
    mWorkedThisMonth = workedThisMonth;
}
