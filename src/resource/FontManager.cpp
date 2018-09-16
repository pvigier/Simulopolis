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
 
#include "resource/FontManager.h"
#include "util/debug.h"
#include "resource/XmlManager.h"

FontManager::FontManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

FontManager::~FontManager()
{
    //dtor
}

void FontManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void FontManager::setUp()
{
    std::string path = mPrefixPath + "fonts.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mFonts[name] = sf::Font();
        mFonts[name].loadFromFile(mPrefixPath + child.getAttributes().get("path"));
    }
}

void FontManager::tearDown()
{

}

void FontManager::addFont(const std::string& name, sf::Font font)
{
    mFonts[name] = std::move(font);
}

const sf::Font& FontManager::getFont(const std::string& name) const
{
    DEBUG_IF(mFonts.find(name) == mFonts.end(), name << " is an invalid font name.\n");
    return mFonts.at(name);
}

FontManager::FontInfo FontManager::getFontInfo(const std::string& name, unsigned int characterSize)
{
    PageId id(name, characterSize);
    std::map<PageId, FontInfo>::iterator it = mInfos.find(id);
    if (it != mInfos.end())
        return it->second;
    else
    {
        FontInfo info = computeFontInfo(name, characterSize);
        mInfos[id] = info;
        return info;
    }
}

FontManager::FontInfo FontManager::computeFontInfo(const std::string& name, unsigned int characterSize)
{
    FontInfo info{0.0f, 0.0f};
    const sf::Font& font = mFonts.at(name);
    for (uint32_t i = 0; i < 256; ++i)
    {
        const sf::Glyph& glyph = font.getGlyph(i, characterSize, false);
        info.maxHeight = std::max(info.maxHeight, glyph.bounds.height);
        info.maxOffset = std::max(info.maxOffset, glyph.bounds.height - std::abs(glyph.bounds.top));
    }
    return info;
}
