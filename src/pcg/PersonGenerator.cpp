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

#include "PersonGenerator.h"
#include <fstream>

PersonGenerator::PersonGenerator(RandomGenerator& generator) : mGenerator(generator)
{

}

void PersonGenerator::setUp()
{
    std::string line;
    std::ifstream file;

    // Male first names
    file.open("media/persons/male_first_names.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mMaleFirstNames.push_back(line);
        file.close();
    }

    // Female first names
    file.open("media/persons/female_first_names.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mFemaleFirstNames.push_back(line);
        file.close();
    }

    // Last names
    file.open("media/persons/last_names.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mLastNames.push_back(line);
        file.close();
    }

    // Cars
    file.open("media/persons/cars.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mCars.push_back(line);
        file.close();
    }
}

std::unique_ptr<Person> PersonGenerator::generate(int year)
{
    // Gender
    std::uniform_int_distribution<int> genderPdf(0, 1);
    Person::Gender gender = static_cast<Person::Gender>(genderPdf(mGenerator));
    // First name
    std::string firstName;
    if (gender == Person::Gender::MALE)
    {
        std::uniform_int_distribution<std::size_t> firstNamePdf(0, mMaleFirstNames.size() - 1);
        firstName = mMaleFirstNames[firstNamePdf(mGenerator)];
    }
    else
    {
        std::uniform_int_distribution<std::size_t> firstNamePdf(0, mFemaleFirstNames.size() - 1);
        firstName = mFemaleFirstNames[firstNamePdf(mGenerator)];
    }
    // Last name
    std::uniform_int_distribution<std::size_t> lastNamePdf(0, mLastNames.size() - 1);
    std::string lastName = mLastNames[lastNamePdf(mGenerator)];
    // Age
    std::uniform_int_distribution<int> agePdf(20, 60);
    int age = agePdf(mGenerator);
    int birth = year - age;
    // Car
    std::uniform_int_distribution<std::size_t> carPdf(0, mCars.size() - 1);
    std::string car = mCars[carPdf(mGenerator)];
    // Personality
    std::array<float, Person::NB_EVALUATORS> biases;
    std::normal_distribution<float> biasPdf(1.0f, 0.2f);
    for (std::size_t i = 0; i < biases.size(); ++i)
        biases[i] = biasPdf(mGenerator);
    return std::make_unique<Person>(firstName, lastName, gender, birth, car, biases);
}
