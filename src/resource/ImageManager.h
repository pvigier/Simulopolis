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
// SFML
#include <SFML/Graphics/Image.hpp>

class XmlManager;

/**
 * \brief Manager that manages the images
 *
 * \author Pierre Vigier
 */
class ImageManager
{
public:
    /**
     * \brief Default constructor
     */
    ImageManager();

    /**
     * \brief Destructor
     */
    ~ImageManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set up the manager
     *
     * Open the "images.xml" file in the folder mPrefixPath and load the
     * images specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a image
     *
     * \param name Name of the image
     * \param image Image to add
     */
    void addImage(const std::string& name, sf::Image image);

    /**
     * \brief Get a image
     *
     * \param name Name of the image to retrieve
     *
     * \return Image that corresponds to name
     */
    const sf::Image& getImage(const std::string& name) const;

private:
    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "images.xml" */
    std::unordered_map<std::string, sf::Image> mImages; /**< Hash map that contains the images */
};
