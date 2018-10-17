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

#include "resource/SaveManager.h"
#include "util/debug.h"
#include "resource/XmlManager.h"
#include "resource/TextureManager.h"
#include "serialize/serialize_city.h"

SaveManager::SaveManager() : mXmlManager(nullptr), mPrefixPath("saves/")
{
    //ctor
}

SaveManager::~SaveManager()
{
    //dtor
}

void SaveManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void SaveManager::setTextureManager(TextureManager* textureManager)
{
    mTextureManager = textureManager;
}

void SaveManager::setUp()
{
    std::string path = mPrefixPath + "saves.xml";
    mDocument = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : mDocument.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mSaves[name] = mPrefixPath + child.getAttributes().get("path");
        sf::Texture texture;
        texture.loadFromFile(mSaves[name] + ".png");
        mTextureManager->addTexture(name, texture);
    }
}

void SaveManager::tearDown()
{

}

void SaveManager::save(const City& city, sf::Texture texture)
{
    const std::string& name = city.getName();
    // Generate a filename if necessary
    std::string path;
    if (hasSave(name))
        path = mSaves[name];
    else
    {
        // Generate a new path
        path = mPrefixPath + name + ".city";
        mSaves[name] = path;
        // Edit the document
        PropertyList attributes;
        attributes.add("name", name);
        attributes.add("path", name + ".city");
        mDocument.addChild(XmlDocument("save", attributes, "", {}));
        updateXmlFile();
    }
    // Save the city
    saveCity(city, path);
    // Save the preview
    std::string previewPath = path + ".png";
    texture.copyToImage().saveToFile(previewPath);
    mTextureManager->addTexture(name, texture);
}

void SaveManager::removeSave(const std::string& name)
{
    if (hasSave(name))
    {
        for (std::size_t i = 0; i < mDocument.getChildren().size(); ++i)
        {
            if (mDocument.getChildren()[i].getAttributes().get("name") == name)
            {
                mDocument.removeChild(i);
                break;
            }
        }
        if (std::remove(mSaves[name].c_str()) != 0)
            DEBUG("Failed to delete " << mSaves[name] << "\n");
        if (std::remove((mSaves[name] + ".png").c_str()) != 0)
            DEBUG("Failed to delete " << mSaves[name] + ".png" << "\n");
        mSaves.erase(name);
        updateXmlFile();
    }
}

void SaveManager::load(const std::string& name, City& city) const
{
    DEBUG_IF(!hasSave(name), name << " is an invalid city name.\n");
    loadCity(city, mSaves.at(name));
}

bool SaveManager::hasSave(const std::string& name) const
{
    return (mSaves.find(name) != mSaves.end());
}

const std::map<std::string, std::string>& SaveManager::getSaves() const
{
    return mSaves;
}

void SaveManager::updateXmlFile()
{
    mXmlManager->saveDocument(mDocument, mPrefixPath + "saves.xml");
}
