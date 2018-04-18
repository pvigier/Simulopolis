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
