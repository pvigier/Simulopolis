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
#include <string>
#include <unordered_map>

class XmlManager;

/**
 * \brief Manager that manages the musics
 *
 * \author Pierre Vigier
 */
class MusicManager
{
public:
    /**
     * \brief Default constructor
     */
    MusicManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set up the manager
     *
     * Open the "musics.xml" file in the folder mPrefixPath and load the
     * musics specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a music
     *
     * \param name Name of the music
     * \param music Music to add
     */
    void addMusic(const std::string& name, std::string music);

    /**
     * \brief Get a music
     *
     * \param name Name of the music to retrieve
     *
     * \return Music that corresponds to name
     */
    const std::string& getMusic(const std::string& name) const;

private:
    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "musics.xml" */
    std::unordered_map<std::string, std::string> mMusics; /**< Hash map that contains the musics */
};
