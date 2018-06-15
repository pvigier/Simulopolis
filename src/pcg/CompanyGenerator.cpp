#include "CompanyGenerator.h"
#include <fstream>
#include <chrono>
#include "city/Person.h"

CompanyGenerator::CompanyGenerator()
{
    //ctor
}

void CompanyGenerator::setUp()
{
    std::string line;
    std::ifstream file;

    // List of fruits
    file.open("media/companies/fruits.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mFruits.push_back(line);
        file.close();
    }

    // List of prefixes
    file.open("media/companies/prefixes.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mPrefixes.push_back(line);
        file.close();
    }

    // List of suffixes
    file.open("media/companies/suffixes.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mSuffixes.push_back(line);
        file.close();
    }

    // List of domains
    file.open("media/companies/domains.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mDomains.push_back(line);
        file.close();
    }
}

Company CompanyGenerator::generate(int year, Person* owner)
{
    std::discrete_distribution<int> typePdf{0.1, 0.4, 0.1, 0.4};
    int type = typePdf(mGenerator);
    std::string name;
    // Fruit
    if (type == 0)
    {
        std::uniform_int_distribution<int> fruitPdf(0, mFruits.size() - 1);
        name += mFruits[fruitPdf(mGenerator)];
    }
    // Owner's name
    else if (type == 1)
        name += owner->getLastName();
    // Prefix + domain
    else if (type == 2)
    {
        std::uniform_int_distribution<int> prefixPdf(0, mPrefixes.size() - 1);
        std::uniform_int_distribution<int> domainPdf(0, mDomains.size() - 1);
        name += mPrefixes[prefixPdf(mGenerator)] + mDomains[domainPdf(mGenerator)];
    }
    // Initials
    else
    {
        std::uniform_int_distribution<int> nbLettersPdf(2, 4);
        std::uniform_int_distribution<int> letterPdf(0, 25);
        int nbLetters = nbLettersPdf(mGenerator);
        name += ('A' + letterPdf(mGenerator));
        if (nbLetters == 2)
            name += '&';
        for (int i = 1; i < nbLetters; ++i)
            name += ('A' + letterPdf(mGenerator));
    }

    // Add the type of the company
    std::uniform_int_distribution<int> suffixPdf(0, mSuffixes.size() - 1);
    std::string suffix = mSuffixes[suffixPdf(mGenerator)];

    return Company(name, suffix, year, owner);
}
