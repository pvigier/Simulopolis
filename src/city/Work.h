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

#pragma once

#include <string>
#include <boost/serialization/access.hpp>
#include "city/Money.h"

class Person;
class Company;
class Building;

class Work
{
public:
    enum class Type{FARMER, WORKER, CRAFTSPERSON, GROCER, CASHIER, SALESPERSON, MANAGER, DOCTOR, TEACHER, POLICE_OFFICER};
    enum class Qualification : int {NON_QUALIFIED = 0, QUALIFIED, HIGHLY_QUALIFIED};

    static std::string typeToString(Type type);

    Work(Type type, Building* workplace);

    Type getType() const;
    Qualification getQualification() const;
    const Person* getEmployee() const;
    std::string getEmployeeName() const;
    void setEmployee(Person* employee);
    const Company* getEmployer() const;
    void setEmployer(Company* employer);
    const Building* getWorkplace() const;
    void setSalary(Money salary);
    Money getSalary() const;
    bool hasWorkedThisMonth() const;
    void setWorkedThisMonth(bool workedThisMonth);

private:
    Type mType;
    Qualification mQualification;
    Person* mEmployee;
    Company* mEmployer;
    Building* mWorkplace;
    Money mSalary;
    bool mWorkedThisMonth;

    // Serialization
    friend class boost::serialization::access;

    Work() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mType & mQualification & mEmployee & mEmployer & mWorkplace & mSalary & mWorkedThisMonth;
    }
};
