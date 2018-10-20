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

#include "resource/TextureManager.h"
#include "util/debug.h"
#include "resource/XmlManager.h"

TextureManager::TextureManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

void TextureManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void TextureManager::setUp()
{
    std::string path = mPrefixPath + "textures.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mTextures[name] = sf::Texture();
        mTextures[name].loadFromFile(mPrefixPath + child.getAttributes().get("path"));
    }
}

void TextureManager::tearDown()
{

}

void TextureManager::addTexture(const std::string& name, sf::Texture texture)
{
    mTextures[name] = std::move(texture);
}

const sf::Texture& TextureManager::getTexture(const std::string& name) const
{
    DEBUG_IF(mTextures.find(name) == mTextures.end(), name << " is an invalid texture name.\n");
    return mTextures.at(name);
}
