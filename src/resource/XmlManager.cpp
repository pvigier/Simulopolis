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

#include "resource/XmlManager.h"
#include "util/debug.h"
#include <sstream>

using namespace tinyxml2;

XmlManager::XmlManager()
{
    //ctor
}

XmlManager::~XmlManager()
{
    //dtor
}

void XmlManager::setUp()
{

}

void XmlManager::tearDown()
{

}

XmlDocument XmlManager::loadDocument(const std::string& path) const
{
    XMLDocument doc;
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        DEBUG(path << " has not been loaded correctly.\n");
        return XmlDocument("", PropertyList(), {});
    }

    return loadDocument(root->ToElement());
}

void XmlManager::saveDocument(const XmlDocument& document, const std::string& path) const
{
    tinyxml2::XMLDocument doc;
    doc.InsertEndChild(createElement(doc, document));
    doc.SaveFile(path.c_str());
}

XmlDocument XmlManager::loadDocument(XMLElement* node) const
{
    std::string name = node->Name();
    PropertyList attributes = createProperties(node);
    std::vector<XmlDocument> children;
    for (XMLElement* child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        children.push_back(loadDocument(child));

    return XmlDocument(name, attributes, children);
}

PropertyList XmlManager::createProperties(XMLElement* node) const
{
    PropertyList properties;
    for (const XMLAttribute* attribute = node->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
        properties.add(attribute->Name(), attribute->Value());
    return std::move(properties);
}

tinyxml2::XMLElement* XmlManager::createElement(tinyxml2::XMLDocument& doc, const XmlDocument& document) const
{
    XMLElement* element = doc.NewElement(document.getName().c_str());
    // Add properties
    for (const std::pair<const std::string, std::string>& property : document.getAttributes().getProperties())
        element->SetAttribute(property.first.c_str(), property.second.c_str());
    // Add children
    for (const XmlDocument& child : document.getChildren())
        element->InsertEndChild(createElement(doc, child));
    return element;
}
