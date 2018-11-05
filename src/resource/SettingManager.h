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

class TextFileManager;

/**
 * \brief Manager that manages the settings
 *
 * \author Pierre Vigier
 */
class SettingManager
{
public:
    /**
     * \brief Default constructor
     */
    SettingManager();

    /**
     * \brief Set text file manager
     *
     * \param textFileManager Text file manager to use
     */
    void setTextFileManager(TextFileManager* textFileManager);

    /**
     * \brief Set up the manager
     *
     * Open the "settings.txt" file in the folder mPrefixPath and load the
     * settings specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    bool isFullscreen() const;
    void setFullscreen(bool fullscreen);

    bool isMusic() const;
    void setMusic(bool music);

private:
    TextFileManager* mTextFileManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "settings.textFile" */
    bool mFullscreen;
    bool mMusic;
};
