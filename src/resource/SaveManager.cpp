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

void SaveManager::setUp()
{
    std::string path = mPrefixPath + "saves.xml";
    mDocument = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : mDocument.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mSaves[name] = mPrefixPath + child.getAttributes().get("path");
    }
}

void SaveManager::tearDown()
{
    mXmlManager->saveDocument(mDocument, mPrefixPath + "saves.xml");
}

void SaveManager::addSave(const std::string& name)
{
    std::string path = mPrefixPath + name + ".city";
    mSaves[name] = path;
    // Edit the document
    PropertyList attributes;
    attributes.add("name", name);
    attributes.add("path", name + ".city");
    mDocument.addChild(XmlDocument("save", attributes, {}));
}

const std::string& SaveManager::getSave(const std::string& name) const
{
    DEBUG_IF(mSaves.find(name) == mSaves.end(), name << " is an invalid city name.\n");
    return mSaves.at(name);
}

bool SaveManager::hasSave(const std::string& name) const
{
    return (mSaves.find(name) != mSaves.end());
}

std::string SaveManager::format(std::string name) const
{

}
