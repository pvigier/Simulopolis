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
#include <memory>

class XmlManager;
class XmlDocument;
class FontManager;

/**
 * \brief Manager that manages the stylesheets
 *
 * \author Pierre Vigier
 */
class StylesheetManager
{
public:
    /**
     * \brief Default constructor
     */
    StylesheetManager();

    /**
     * \brief Destructor
     */
    ~StylesheetManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set up the manager
     *
     * Open the "stylesheets.xml" file in the folder mPrefixPath and load the
     * stylesheets specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a stylesheet
     *
     * \param name Name of the stylesheet
     * \param stylesheet Stylesheet to add
     */
    void addStylesheet(const std::string& name, XmlDocument stylesheet);

    /**
     * \brief Get a stylesheet
     *
     * \param name Name of the stylesheet to retrieve
     *
     * \return Stylesheet that corresponds to name
     */
    const XmlDocument* getStylesheet(const std::string& name) const;

private:
    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "stylesheets.xml" */
    std::unordered_map<std::string, std::unique_ptr<XmlDocument>> mStylesheets; /**< Hash map that contains the stylesheets */
};
