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
#include <SFML/Graphics/Texture.hpp>

class XmlManager;

/**
 * \brief Manager that manages the textures
 *
 * \author Pierre Vigier
 */
class TextureManager
{
public:
    /**
     * \brief Default constructor
     */
    TextureManager();

    /**
     * \brief Destructor
     */
    ~TextureManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set up the manager
     *
     * Open the "textures.xml" file in the folder mPrefixPath and load the
     * textures specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a texture
     *
     * \param name Name of the texture
     * \param texture Texture to add
     */
    void addTexture(const std::string& name, sf::Texture texture);

    /**
     * \brief Get a texture
     *
     * \param name Name of the texture to retrieve
     *
     * \return Texture that corresponds to name
     */
    const sf::Texture& getTexture(const std::string& name) const;

private:
    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "textures.xml" */
    std::unordered_map<std::string, sf::Texture> mTextures; /**< Hash map that contains the textures */
};
