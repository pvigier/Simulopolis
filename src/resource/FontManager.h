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
#include <map>
// SFML
#include <SFML/Graphics/Font.hpp>

class XmlManager;

/**
 * \brief Manager that manages the fonts
 *
 * \author Pierre Vigier
 */
class FontManager
{
public:
    struct FontInfo
    {
        float maxHeight;
        float maxOffset;
    };

    /**
     * \brief Default constructor
     */
    FontManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set up the manager
     *
     * Open the "fonts.xml" file in the folder mPrefixPath and load the
     * fonts specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a font
     *
     * \param name Name of the font
     * \param font Font to add
     */
    void addFont(const std::string& name, sf::Font font);

    /**
     * \brief Get a font
     *
     * \param name Name of the font to retrieve
     *
     * \return Font that corresponds to name
     */
    const sf::Font& getFont(const std::string& name) const;

    FontInfo getFontInfo(const std::string& name, unsigned int characterSize);

private:
    using PageId = std::tuple<std::string, unsigned int>;

    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "fonts.xml" */
    std::unordered_map<std::string, sf::Font> mFonts; /**< Hash map that contains the fonts */
    std::map<PageId, FontInfo> mInfos; /**< Hash map that contains info about the font */

    FontInfo computeFontInfo(const std::string& name, unsigned int characterSize);
};
