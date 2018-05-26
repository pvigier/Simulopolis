#pragma once

// My includes
#include "resource/XmlManager.h"
#include "resource/TextureManager.h"
#include "resource/FontManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "resource/MusicManager.h"

/**
 * \brief Manager that manages the specific resource managers
 *
 * \author Pierre Vigier
 */
class ResourceManager
{
public:
    /**
     * \brief Default constructor
     */
    ResourceManager();

    /**
     * \brief Destructor
     */
    ~ResourceManager();

    /**
     * \brief Set up the specific resource managers
     */
    void setUp();

    /**
     * \brief Tear down the specific resource managers
     */
    void tearDown();

    /**
     * \brief Get the xml manager
     *
     * \return The xml manager
     */
    XmlManager& getXmlManager();

    /**
     * \brief Get the texture manager
     *
     * \return The texture manager
     */
    TextureManager& getTextureManager();

    /**
     * \brief Get the font manager
     *
     * \return The font manager
     */
    FontManager& getFontManager();

    /**
     * \brief Get the stylesheet manager
     *
     * \return The stylesheet manager
     */
    StylesheetManager& getStylesheetManager();

    /**
     * \brief Get the gui manager
     *
     * \return The gui manager
     */
    GuiManager& getGuiManager();

    /**
     * \brief Get the music manager
     *
     * \return The music manager
     */
    MusicManager& getMusicManager();

private:
    XmlManager mXmlManager; /**< Xml manager */
    TextureManager mTextureManager; /**< Texture manager */
    FontManager mFontManager; /**< Font manager */
    StylesheetManager mStylesheetManager; /**< Stylesheet manager */
    GuiManager mGuiManager; /**< Gui manager */
    MusicManager mMusicManager; /**< Music manager */
};
