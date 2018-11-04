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

#include "resource/TextFileManager.h"
#include <fstream>
#include <regex>
#include "util/debug.h"

TextFileManager::TextFileManager()
{
    
}

void TextFileManager::setUp()
{

}

void TextFileManager::tearDown()
{

}

std::vector<std::string> TextFileManager::loadValues(const std::string& path) const
{
    std::ifstream file; 
    file.open(path);
    if (!file.is_open())
    {
        DEBUG(path << " has not been loaded correctly.\n");
        return {};
    }

    std::vector<std::string> values;
    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty() && line[0] != '#')
            values.emplace_back(std::move(line));
    }
    file.close();

    return values;
}

std::unordered_map<std::string, std::string> TextFileManager::loadDictionary(const std::string& path) const
{
    std::ifstream file; 
    file.open(path);
    if (!file.is_open())
    {
        DEBUG(path << " has not been loaded correctly.\n");
        return {};
    }

    std::regex regex(R"((\S*)\s->\s(\S*))");
    std::unordered_map<std::string, std::string> dictionary;
    std::string line;
    std::size_t i = 0;
    while (std::getline(file, line))
    {
        if (!line.empty() && line[0] != '#')
        {
            std::smatch match;
            bool valid = std::regex_match(line, match, regex);
            if (!valid)
                DEBUG("Line " << i << " in " << path << " is not a valid key-value pair.\n");
            else
            {
                std::string key = match[1];
                std::string value = match[2];
                dictionary.emplace(std::move(key), std::move(value));
            }
        }
        ++i;
    }
    file.close();

    return dictionary;
}

