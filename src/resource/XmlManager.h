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
#include <unordered_map>
// tinyxml2
#include <tinyxml2.h>
// My includes
#include "resource/XmlDocument.h"

/**
 * \brief Manager that manages xml documents
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
     * \brief Set up the manager
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Load a document
     *
     * \param path Path of the document
     *
     * \return The loaded document
     */
    XmlDocument loadDocument(const std::string& path) const;

    /**
     * \brief Save a document
     *
     * \param document Document to save
     * \param path Path of the document
     *
     */
    void saveDocument(const XmlDocument& document, const std::string& path) const;

private:
    XmlDocument loadDocument(tinyxml2::XMLElement* node) const;
    PropertyList createProperties(tinyxml2::XMLElement* node) const;
    tinyxml2::XMLElement* createElement(tinyxml2::XMLDocument& doc, const XmlDocument& document) const;
};
