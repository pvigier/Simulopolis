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
