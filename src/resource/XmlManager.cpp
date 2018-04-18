#include "resource/XmlManager.h"
#include <iostream>
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

XmlDocument* XmlManager::loadDocument(const std::string& name, const std::string& path)
{
    XMLDocument doc;
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        std::cout << path << " has not been loaded correctly." << std::endl;
        return nullptr;
    }

    mDocuments[name] = std::make_unique<XmlDocument>(loadDocument(root->ToElement()));
    return getDocument(name);
}

XmlDocument* XmlManager::getDocument(const std::string& name)
{
    return mDocuments[name].get();
}

XmlDocument XmlManager::loadDocument(XMLElement* node)
{
    std::string name = node->Name();
    PropertyList attributes = createProperties(node);
    std::vector<XmlDocument> children;
    for (XMLElement* child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        children.push_back(loadDocument(child));

    return std::move(XmlDocument(name, attributes, children));
}

PropertyList XmlManager::createProperties(XMLElement* node)
{
    PropertyList properties;
    for (const XMLAttribute* attribute = node->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
        properties.add(attribute->Name(), attribute->Value());
    return std::move(properties);
}
