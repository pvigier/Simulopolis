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
 
#include "resource/MusicManager.h"
#include "util/debug.h"
#include "resource/XmlManager.h"

MusicManager::MusicManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

MusicManager::~MusicManager()
{
    //dtor
}

void MusicManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void MusicManager::setUp()
{
    std::string path = mPrefixPath + "musics.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mMusics[name] = mPrefixPath + child.getAttributes().get("path");
    }
}

void MusicManager::tearDown()
{

}

void MusicManager::addMusic(const std::string& name, std::string music)
{
    mMusics[name] = std::move(music);
}

const std::string& MusicManager::getMusic(const std::string& name) const
{
    DEBUG_IF(mMusics.find(name) == mMusics.end(), name << " is an invalid music name.\n");
    return mMusics.at(name);
}
