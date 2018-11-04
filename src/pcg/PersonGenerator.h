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

#include <memory>
#include <vector>

class RandomGenerator;
class TextFileManager;
class Person;

class PersonGenerator
{
public:
    static void setTextFileManager(TextFileManager* textFileManager);

    PersonGenerator(RandomGenerator& generator);

    void setUp();

    std::unique_ptr<Person> generate(int year);
    std::string generateName();

private:
    static TextFileManager* sTextFileManager;
    RandomGenerator& mGenerator;
    std::vector<std::string> mMaleFirstNames;
    std::vector<std::string> mFemaleFirstNames;
    std::vector<std::string> mLastNames;
    std::vector<std::string> mCars;
};
