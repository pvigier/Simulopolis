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
// My includes
#include "resource/XmlDocument.h"
#include "gui/Gui.h"
#include "gui/GuiLayout.h"

class XmlManager;
class GuiWidget;

/**
 * \brief Manager that manages the gui's
 *
 * \author Pierre Vigier
 */
class GuiManager
{
public:
    /**
     * \brief Default constructor
     */
    GuiManager();

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
     * gui's specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a gui
     *
     * \param name Name of the gui
     * \param gui Gui to add
     */
    void addGui(const std::string& name, XmlDocument gui);

    /**
     * \brief Get a gui
     *
     * \param name Name of the gui to retrieve
     *
     * \return Gui that corresponds to name
     */
    std::unique_ptr<Gui> getGui(const std::string& name);

private:
    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "guis.xml" */
    std::unordered_map<std::string, std::unique_ptr<XmlDocument>> mGuis; /**< Hash map that contains the gui's */

    /**
     * \brief Load a gui from a XML node
     *
     * \param node XML node that describes the gui
     */
    void loadGui(const XmlDocument& node);

    void loadRootWidgets(Gui* gui, const XmlDocument& node);
    std::unique_ptr<GuiWidget> loadWidgets(Gui* gui, const XmlDocument& node);
    bool isWidget(const XmlDocument& node);
    bool isLayout(const XmlDocument& node);
    std::unique_ptr<GuiWidget> createWidget(const XmlDocument& node);
    std::unique_ptr<GuiLayout> createLayout(const XmlDocument& node);
};
