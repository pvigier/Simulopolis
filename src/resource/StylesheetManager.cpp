#include "StylesheetManager.h"
#include <iostream>
#include "resource/FontManager.h"

using namespace tinyxml2;

FontManager* StylesheetManager::sFontManager = nullptr;

StylesheetManager::StylesheetManager() : mPrefixPath("media/")
{
    //ctor
}

StylesheetManager::~StylesheetManager()
{
    //dtor
}

void StylesheetManager::setUp()
{
    XMLDocument doc;
    std::string path = mPrefixPath + "stylesheets.xml";
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        std::cout << mPrefixPath + "stylesheets.xml" << " has not been loaded correctly." << std::endl;
        return;
    }

    // Stylesheets
    for (XMLElement* node = root->FirstChildElement("stylesheet"); node != nullptr; node = node->NextSiblingElement("stylesheet"))
        loadStylesheet(node);
}

void StylesheetManager::tearDown()
{

}

void StylesheetManager::addStylesheet(const std::string& name, GuiStyle stylesheet)
{
    mStylesheets[name] = stylesheet;
}

GuiStyle& StylesheetManager::getStylesheet(const std::string& name) const
{
    return mStylesheets[name];
}

void StylesheetManager::setFontManager(FontManager* fontManager)
{
    sFontManager = fontManager;
}

void StylesheetManager::loadStylesheet(XMLElement* node)
{
    // Name
    std::string name = node->Attribute("name");

    // Body
    XMLElement* bodyNode = node->FirstChildElement("body");
    sf::Color bodyColor = stringToColor(bodyNode->Attribute("color"));
    sf::Color bodyHighlightColor = stringToColor(bodyNode->Attribute("highlightColor"));

    // Border
    XMLElement* borderNode = node->FirstChildElement("border");
    float borderSize = borderNode->FloatAttribute("size");
    sf::Color borderColor = stringToColor(borderNode->Attribute("color"));
    sf::Color borderHighlightColor = stringToColor(borderNode->Attribute("highlightColor"));

    // Text
    XMLElement* textNode = node->FirstChildElement("text");
    const sf::Font* font = &sFontManager->getFont(textNode->Attribute("font"));
    sf::Color textColor = stringToColor(textNode->Attribute("color"));
    sf::Color textHighlightColor = stringToColor(textNode->Attribute("highlightColor"));

    mStylesheets[name] = GuiStyle(bodyColor, bodyHighlightColor, borderColor, borderHighlightColor,
        borderSize, textColor, textHighlightColor, font);
}

sf::Color StylesheetManager::stringToColor(const std::string& s) const
{
    std::istringstream stream(s);
    std::string r, g, b;
    stream >> r >> g >> b;
    return sf::Color(std::stoi(r, nullptr, 16), std::stoi(g, nullptr, 16, std::stoi(b, nullptr, 16));
}
