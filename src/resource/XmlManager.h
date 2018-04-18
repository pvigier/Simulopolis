#pragma once

// STL
#include <unordered_map>
// tinyxml2
#include <tinyxml2.h>
// My includes
#include "resource/XmlDocument.h"

/**
 * \brief Manager that manages the gui's
 *
 * \author Pierre Vigier
 */
class XmlManager
{
public:
    /**
     * \brief Default constructor
     */
    XmlManager();

    /**
     * \brief Destructor
     */
    ~XmlManager();

    /**
     * \brief Set up the manager
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Load and add a document
     *
     * \param name Name of the document
     * \param path Path of the document
     *
     * \return Return the loaded document
     */
    XmlDocument* loadDocument(const std::string& name, const std::string& path);

    /**
     * \brief Get a document
     *
     * \param name Name of the document to retrieve
     *
     * \return XmlDocument that corresponds to name
     */
    XmlDocument* getDocument(const std::string& name);

private:
    std::unordered_map<std::string, std::unique_ptr<XmlDocument>> mDocuments; /**< Hash map that contains the documents */

    XmlDocument loadDocument(tinyxml2::XMLElement* node);
    PropertyList createProperties(tinyxml2::XMLElement* node);

    sf::Vector2f stringToVector(const std::string& s) const;
};
