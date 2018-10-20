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

// My includes
#include "resource/XmlManager.h"
#include "resource/TextureManager.h"
#include "resource/FontManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "resource/MusicManager.h"
#include "resource/ImageManager.h"
#include "resource/SaveManager.h"

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

    /**
     * \brief Get the image manager
     *
     * \return The image manager
     */
    ImageManager& getImageManager();

    /**
     * \brief Get the save manager
     *
     * \return The save manager
     */
    SaveManager& getSaveManager();

private:
    XmlManager mXmlManager; /**< Xml manager */
    TextureManager mTextureManager; /**< Texture manager */
    FontManager mFontManager; /**< Font manager */
    StylesheetManager mStylesheetManager; /**< Stylesheet manager */
    GuiManager mGuiManager; /**< Gui manager */
    MusicManager mMusicManager; /**< Music manager */
    ImageManager mImageManager; /**< Image manager */
    SaveManager mSaveManager; /**< Save manager */
};
