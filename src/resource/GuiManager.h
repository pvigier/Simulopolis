#pragma once

// STL
#include <string>
#include <unordered_map>
// tinyxml2
#include <tinyxml2.h>
// My includes
#include "gui/Gui.h"
#include "gui/GuiLayout.h"
#include "resource/PropertyList.h"

class TextureManager;
class FontManager;
class StylesheetManager;
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
     * \brief Destructor
     */
    ~GuiManager();

    /**
     * \brief Set texture manager
     *
     * \param textureManager TextureManager to use
     */
    void setTextureManager(TextureManager* textureManager);

    /**
     * \brief Set font manager
     *
     * \param fontManager FontManager to use
     */
    void setFontManager(FontManager* fontManager);

    /**
     * \brief Set stylesheet manager
     *
     * \param stylesheeManager StylesheetManager to use
     */
    void setStylesheetManager(StylesheetManager* stylesheetManager);

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
    void addGui(const std::string& name, std::unique_ptr<Gui> gui);

    /**
     * \brief Get a gui
     *
     * \param name Name of the gui to retrieve
     *
     * \return Gui that corresponds to name
     */
    Gui* getGui(const std::string& name);

private:
    TextureManager* mTextureManager;
    FontManager* mFontManager;
    StylesheetManager* mStylesheetManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "guis.xml" */
    std::unordered_map<std::string, std::unique_ptr<Gui>> mGuis; /**< Hash map that contains the gui's */

    /**
     * \brief Load a stylesheet from a XML node
     *
     * \param node XML node that describes the stylesheet
     */
    void loadGui(tinyxml2::XMLElement* node);

    void loadRootWidget(Gui* gui, tinyxml2::XMLNode* node);
    std::unique_ptr<GuiWidget> loadWidget(Gui* gui, tinyxml2::XMLElement* node);
    bool isWidget(tinyxml2::XMLElement* node);
    bool isLayout(tinyxml2::XMLElement* node);
    std::unique_ptr<GuiWidget> createWidget(tinyxml2::XMLElement* node);
    std::unique_ptr<GuiLayout> createLayout(tinyxml2::XMLElement* node);
    PropertyList createProperties(tinyxml2::XMLElement* node);

    sf::Vector2f stringToVector(const std::string& s) const;
};
