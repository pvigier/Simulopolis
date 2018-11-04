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

#include "pcg/PersonGenerator.h"
#include "resource/TextFileManager.h"
#include "pcg/RandomGenerator.h"
#include "city/Person.h"

TextFileManager* PersonGenerator::sTextFileManager = nullptr;
    
void PersonGenerator::setTextFileManager(TextFileManager* textFileManager)
{
    sTextFileManager = textFileManager;
}

PersonGenerator::PersonGenerator(RandomGenerator& generator) : mGenerator(generator)
{

}

void PersonGenerator::setUp()
{
    mMaleFirstNames = sTextFileManager->loadValues("media/persons/male_first_names.txt");
    mFemaleFirstNames = sTextFileManager->loadValues("media/persons/female_first_names.txt");
    mLastNames = sTextFileManager->loadValues("media/persons/last_names.txt");
    mCars = sTextFileManager->loadValues("media/persons/cars.txt");
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
    // Physiology
    static std::array<float, static_cast<int>(Person::Need::COUNT)> standardDecayRates =
        {0.1f, 0.1f, 0.01f, 0.01f, 0.1f};
    std::array<float, static_cast<int>(Person::Need::COUNT)> decayRates;
    std::normal_distribution<float> decayRatePdf(1.0f, 0.2f);
    for (std::size_t i = 0; i < decayRates.size(); ++i)
        decayRates[i] = std::max(0.0f, decayRatePdf(mGenerator)) * standardDecayRates[i];
    // Abilities
    std::normal_distribution<double> producivityPdf(1.0, 0.2);
    double productivity = std::max(0.0, producivityPdf(mGenerator));
    // Personality
    std::array<float, Person::NB_EVALUATORS> biases;
    std::normal_distribution<float> biasPdf(1.0f, 0.2f);
    for (std::size_t i = 0; i < biases.size(); ++i)
        biases[i] = std::max(0.0f, biasPdf(mGenerator));
    // Funds
    std::exponential_distribution<double> fundsPdf(1 / 100.0); // 100.0f is the mean
    Money funds(fundsPdf(mGenerator));
    return std::make_unique<Person>(firstName, lastName, gender, birth, car, decayRates, productivity, biases, funds);
}

std::string PersonGenerator::generateName()
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
    return firstName + ' ' + lastName;
}
