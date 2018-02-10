#pragma once

// My includes
#include "resource/TextureManager.h"
#include "resource/FontManager.h"
#include "resource/StylesheetManager.h"

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

private:
    TextureManager mTextureManager; /**< Texture manager */
    FontManager mFontManager; /**< Font manager */
    StylesheetManager mStylesheetManager; /**< Stylesheet manager */
};
