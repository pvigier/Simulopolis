#include "resource/StylesheetManager.h"
#include <iostream>
#include <sstream>
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
    return mStylesheets.at(name).get();
}
