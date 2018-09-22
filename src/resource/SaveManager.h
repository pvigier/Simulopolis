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
// XmlDocument
#include "resource/XmlDocument.h"

class XmlManager;
class TextureManager;

/**
 * \brief Manager that manages the saves
 *
 * \author Pierre Vigier
 */
class SaveManager
{
public:
    /**
     * \brief Default constructor
     */
    SaveManager();

    /**
     * \brief Destructor
     */
    ~SaveManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set texture manager
     *
     * \param textureManager TextureManager to use
     */
    void setTextureManager(TextureManager* textureManager);

    /**
     * \brief Set up the manager
     *
     * Open the "saves.xml" file in the folder mPrefixPath
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a city
     *
     * \param name Name of the city
     */
    void addSave(const std::string& name);

    /**
     * \brief Remove a city
     *
     * \param name Name of the city
     */
    void removeSave(const std::string& name);

    /**
     * \brief Get a save
     *
     * \param name Name of the city to retrieve
     *
     * \return Path to the save that corresponds to name
     */
    const std::string& getSave(const std::string& name) const;

    /**
     * \brief Check if a city already exists
     *
     * \param name Name of the city to retrieve
     *
     * \return True if the city already exists, false otherwise
     */
    bool hasSave(const std::string& name) const;

    /**
     * \brief Return all the saves
     *
     * \return All the saves
     */
    const std::unordered_map<std::string, std::string>& getSaves() const;

    /**
     * \brief Update the xml file
     */
     void updateXmlFile();

private:
    XmlManager* mXmlManager;
    TextureManager* mTextureManager;
    XmlDocument mDocument; /**< Document where the saves are stored on the disk */
    std::string mPrefixPath; /**< Path of the folder in which is located "saves.xml" */
    std::unordered_map<std::string, std::string> mSaves; /**< Hash map that contains the saves */
};
