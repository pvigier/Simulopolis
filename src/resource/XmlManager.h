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
     * \return The loaded document
     */
    XmlDocument loadDocument(const std::string& path);

private:
    XmlDocument loadDocument(tinyxml2::XMLElement* node);
    PropertyList createProperties(tinyxml2::XMLElement* node);
};
