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
 
#include "resource/ImageManager.h"
#include "util/debug.h"
#include "resource/XmlManager.h"

ImageManager::ImageManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

ImageManager::~ImageManager()
{
    //dtor
}

void ImageManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void ImageManager::setUp()
{
    std::string path = mPrefixPath + "images.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mImages[name] = sf::Image();
        mImages[name].loadFromFile(mPrefixPath + child.getAttributes().get("path"));
    }
}

void ImageManager::tearDown()
{

}

void ImageManager::addImage(const std::string& name, sf::Image image)
{
    mImages[name] = std::move(image);
}

const sf::Image& ImageManager::getImage(const std::string& name) const
{
    DEBUG_IF(mImages.find(name) == mImages.end(), name << " is an invalid image name.\n");
    return mImages.at(name);
}
