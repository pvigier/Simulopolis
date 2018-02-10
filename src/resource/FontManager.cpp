#include "FontManager.h"
#include <iostream>

using namespace tinyxml2;

FontManager::FontManager() : mPrefixPath("media/")
{
    //ctor
}

FontManager::~FontManager()
{
    //dtor
}

void FontManager::setUp()
{
    XMLDocument doc;
    std::string path = mPrefixPath + "fonts.xml";
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        std::cout << mPrefixPath + "fonts.xml" << " has not been loaded correctly." << std::endl;
        return;
    }

    // Fonts
    for (XMLElement* node = root->FirstChildElement("font"); node != nullptr; node = node->NextSiblingElement("font"))
        loadFont(node);
}

void FontManager::tearDown()
{

}

void FontManager::addFont(const std::string& name, sf::Font font)
{
    mFonts[name] = font;
}

sf::Font& FontManager::getFont(const std::string& name) const
{
    return mFonts[name];
}

void FontManager::loadFont(XMLElement* node)
{
    // Parameters
    std::string name = node->Attribute("name");
    // Path
    std::string path = node->Attribute("path");
    mFonts[name] = sf::Font();
    mFonts[name].loadFromFile(mPrefixPath + path);
}
