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
 
#include "resource/StylesheetManager.h"
#include "util/debug.h"
#include "resource/XmlManager.h"

using namespace tinyxml2;

StylesheetManager::StylesheetManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

StylesheetManager::~StylesheetManager()
{
    //dtor
}

void StylesheetManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void StylesheetManager::setUp()
{
    std::string path = mPrefixPath + "stylesheets.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mStylesheets[name] = std::make_unique<XmlDocument>(child);
    }
}

void StylesheetManager::tearDown()
{

}

void StylesheetManager::addStylesheet(const std::string& name, XmlDocument stylesheet)
{
    mStylesheets[name] = std::make_unique<XmlDocument>(std::move(stylesheet));
}

const XmlDocument* StylesheetManager::getStylesheet(const std::string& name) const
{
    DEBUG_IF(mStylesheets.find(name) == mStylesheets.end(), name << " is an invalid stylesheet name.\n");
    return mStylesheets.at(name).get();
}
