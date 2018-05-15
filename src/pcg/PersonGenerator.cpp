#include "PersonGenerator.h"

PersonGenerator::PersonGenerator() : mGenerator(std::chrono::system_clock::now().time_since_epoch().count())
{

}

void PersonGenerator::setUp()
{
    std::string line;
    std::ifstream file;

    // Male first names
    file.open("media/male_first_names.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mMaleFirstNames.push_back(line);
        file.close();
    }

    // Female first names
    file.open("media/female_first_names.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mFemaleFirstNames.push_back(line);
        file.close();
    }

    // Last names
    file.open("media/last_names.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mLastNames.push_back(line);
        file.close();
    }
}

Person PersonGenerator::generate(int year)
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
    return Person(firstName, lastName, gender, birth);
}
