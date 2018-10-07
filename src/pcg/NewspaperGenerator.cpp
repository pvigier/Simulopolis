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

#include "NewspaperGenerator.h"
#include <fstream>
#include "pcg/RandomGenerator.h"
#include "city/Company.h"
#include "city/Person.h"

NewspaperGenerator::NewspaperGenerator(RandomGenerator& generator) : mGenerator(generator)
{
    //ctor
}

void NewspaperGenerator::setUp()
{
    std::string line;
    std::ifstream file;

    // List of patterns
    file.open("media/newspapers/patterns.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
            mPatterns.push_back(line);
        file.close();
    }
}

std::string NewspaperGenerator::generate(const std::string& cityName)
{
    std::uniform_int_distribution<int> patternPdf(0, mPatterns.size() - 1);
    std::string name = mPatterns[patternPdf(mGenerator)];
    return name.replace(name.find("<city>"), 6, cityName);
}
