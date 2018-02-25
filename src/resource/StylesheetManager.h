#pragma once

// STL
#include <string>
#include <unordered_map>
// tinyxml2
#include <tinyxml2.h>
// My includes
#include "gui/GuiStyle.h"

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
     * \brief Set font manager
     *
     * \param fontManager FontManager to use
     */
    void setFontManager(FontManager* fontManager);

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
    void addStylesheet(const std::string& name, GuiStyle stylesheet);

    /**
     * \brief Get a stylesheet
     *
     * \param name Name of the stylesheet to retrieve
     *
     * \return Stylesheet that corresponds to name
     */
    const GuiStyle& getStylesheet(const std::string& name) const;

private:
    FontManager* mFontManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "stylesheets.xml" */
    std::unordered_map<std::string, GuiStyle> mStylesheets; /**< Hash map that contains the stylesheets */

    /**
     * \brief Load a stylesheet from a XML node
     *
     * \param node XML node that describes the stylesheet
     */
    void loadStylesheet(tinyxml2::XMLElement* node);
    sf::Color stringToColor(const std::string& s) const;
};
