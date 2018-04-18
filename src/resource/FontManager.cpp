#include "resource/FontManager.h"
#include "resource/XmlManager.h"
#include <iostream>

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
    mFonts[name] = font;
}

const sf::Font& FontManager::getFont(const std::string& name) const
{
    return mFonts.at(name);
}
