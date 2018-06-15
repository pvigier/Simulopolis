#include "PersonGenerator.h"
#include <fstream>
#include <chrono>

PersonGenerator::PersonGenerator() : mGenerator(std::chrono::system_clock::now().time_since_epoch().count())
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
    std::uniform_int_distribution<int> genderPdf(0, 1);
    Person::Gender gender = static_cast<Person::Gender>(genderPdf(mGenerator));
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
    std::uniform_int_distribution<std::size_t> lastNamePdf(0, mLastNames.size() - 1);
    std::string lastName = mLastNames[lastNamePdf(mGenerator)];
    std::uniform_int_distribution<int> agePdf(20, 60);
    int age = agePdf(mGenerator);
    int birth = year - age;
    std::uniform_int_distribution<std::size_t> carPdf(0, mCars.size() - 1);
    std::string car = mCars[carPdf(mGenerator)];
    return std::make_unique<Person>(firstName, lastName, gender, birth, car);
}
