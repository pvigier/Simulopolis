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

std::string Work::typeToString(WorkType type)
{
    switch (type)
    {
        case WorkType::FARMER:
            return "Farmer";
        case WorkType::WORKER:
            return "Worker";
        case WorkType::CRAFTSPERSON:
            return "Craftsperson";
        case WorkType::GROCER:
            return "Grocer";
        case WorkType::CASHIER:
            return "Cashier";
        case WorkType::SALESPERSON:
            return "Salesperson";
        case WorkType::MANAGER:
            return "Manager";
        case WorkType::DOCTOR:
            return "Doctor";
        case WorkType::TEACHER:
            return "Teacher";
        case WorkType::POLICE_OFFICER:
            return "Police officer";
        default:
            return "";
    }
}

Qualification Work::typeToQualification(WorkType type)
{
    switch (type)
    {
        case WorkType::FARMER:
        case WorkType::GROCER:
        case WorkType::CASHIER:
        case WorkType::WORKER:
            return Qualification::NON_QUALIFIED;
        case WorkType::CRAFTSPERSON:
        case WorkType::SALESPERSON:
        case WorkType::TEACHER:
        case WorkType::POLICE_OFFICER:
            return Qualification::QUALIFIED;
        case WorkType::MANAGER:
        case WorkType::DOCTOR:
            return Qualification::HIGHLY_QUALIFIED;
        default:
            return Qualification::NON_QUALIFIED;
    }
}

float Work::typeToArduousness(WorkType type)
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

Work::Work(WorkType type, Building* workplace) :
    mType(type), mQualification(typeToQualification(mType)),
    mEmployee(nullptr), mEmployer(nullptr), mWorkplace(workplace),
    mSalary(0.0), mArduousness(typeToArduousness(mType)), mWorkedThisMonth(false)
{

}

WorkType Work::getType() const
{
    return mType;
}

Qualification Work::getQualification() const
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
