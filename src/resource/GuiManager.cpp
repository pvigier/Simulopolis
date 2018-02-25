#include "resource/GuiManager.h"
#include <iostream>
#include <sstream>
#include "resource/TextureManager.h"
#include "resource/FontManager.h"
#include "resource/StylesheetManager.h"

using namespace tinyxml2;

GuiManager::GuiManager() : mTextureManager(nullptr), mFontManager(nullptr), mStylesheetManager(nullptr),
    mPrefixPath("media/")
{
    //ctor
}

GuiManager::~GuiManager()
{
    //dtor
}

void GuiManager::setTextureManager(TextureManager* textureManager)
{
    mTextureManager = textureManager;
}

void GuiManager::setFontManager(FontManager* fontManager)
{
    mFontManager = fontManager;
}

void GuiManager::setStylesheetManager(StylesheetManager* stylesheetManager)
{
    mStylesheetManager = stylesheetManager;
}

void GuiManager::setUp()
{
    XMLDocument doc;
    std::string path = mPrefixPath + "guis.xml";
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        std::cout << mPrefixPath + "guis.xml" << " has not been loaded correctly." << std::endl;
        return;
    }

    // Gui's
    for (XMLElement* node = root->FirstChildElement("gui"); node != nullptr; node = node->NextSiblingElement("gui"))
        loadGui(node);
}

void GuiManager::tearDown()
{

}

void GuiManager::addGui(const std::string& name, std::shared_ptr<Gui> gui)
{
    mGuis[name] = gui;
}

std::shared_ptr<Gui> GuiManager::getGui(const std::string& name)
{
    return mGuis[name];
}

void GuiManager::loadGui(XMLElement* node)
{
    // Name and path
    std::string name = node->Attribute("name");
    std::string path = mPrefixPath + node->Attribute("path");

    // Load the gui
    XMLDocument doc;
    std::cout << path << std::endl;
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        std::cout << path << " has not been loaded correctly." << std::endl;
        return;
    }

    // Gui's
    /*for (XMLElement* node = root->FirstChildElement("gui"); node != nullptr; node = node->NextSiblingElement("gui"))
        loadGui(node);*/
}
