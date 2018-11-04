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

// STL
#include <vector>
#include <unordered_map>

/**
 * \brief Manager that manages text file documents 
 *
 * \author Pierre Vigier
 */
class TextFileManager
{
public:
    /**
     * \brief Default constructor
     */
    TextFileManager();

    /**
     * \brief Set up the manager
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Load values from a text file
     *
     * \param path Path of the document
     *
     * \return The loaded values
     */
    std::vector<std::string> loadValues(const std::string& path) const;

    /**
     * \brief Load values from a text file
     *
     * \param path Path of the document
     *
     * \return The loaded values
     */
    std::unordered_map<std::string, std::string> loadDictionary(const std::string& path) const;
};
