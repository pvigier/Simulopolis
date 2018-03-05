#include "resource/GuiManager.h"
#include <iostream>
#include <sstream>
#include "resource/TextureManager.h"
#include "resource/FontManager.h"
#include "resource/StylesheetManager.h"
#include "resource/PropertyList.h"
#include "gui/GuiWidget.h"
#include "gui/GuiButton.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiHBoxLayout.h"
#include "gui/GuiVBoxLayout.h"

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

void GuiManager::addGui(const std::string& name, std::unique_ptr<Gui> gui)
{
    mGuis[name] = std::move(gui);
}

Gui* GuiManager::getGui(const std::string& name)
{
    return mGuis[name].get();
}

void GuiManager::loadGui(XMLElement* node)
{
    // Name and path
    std::string name = node->Attribute("name");
    std::string path = mPrefixPath + node->Attribute("path");

    // Load the gui
    XMLDocument doc;
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        std::cout << path << " has not been loaded correctly." << std::endl;
        return;
    }

    // Gui's
    std::unique_ptr<Gui> gui(new Gui());
    loadRootWidget(gui.get(), root);
    mGuis[name] = std::move(gui);
}

void GuiManager::loadRootWidget(Gui* gui, XMLNode* node)
{
    for (XMLElement* child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        gui->addRoot(child->Attribute("name"), loadWidget(gui, child));
}

std::unique_ptr<GuiWidget> GuiManager::loadWidget(Gui* gui, XMLElement* node)
{
    // Create widget
    std::unique_ptr<GuiWidget> widget = createWidget(node);
    // Create children
    for (XMLElement* child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (isWidget(child))
        {
            std::unique_ptr<GuiWidget> childWidget = loadWidget(gui, child);
            widget->add(childWidget.get());
            gui->add(child->Attribute("name"), std::move(childWidget));
        }
        else if (isLayout(child))
            widget->setLayout(createLayout(child));
    }
    return widget;
}

bool GuiManager::isWidget(tinyxml2::XMLElement* node)
{
    std::string type = node->Name();
    return (type == "widget" || type == "button" || type == "text" || type == "image");
}

bool GuiManager::isLayout(tinyxml2::XMLElement* node)
{
    std::string type = node->Name();
    return (type == "hboxlayout" || type == "vboxlayout");
}

std::unique_ptr<GuiWidget> GuiManager::createWidget(tinyxml2::XMLElement* node)
{
    std::unique_ptr<GuiWidget> widget;
    std::string type = node->Name();
    PropertyList properties = createProperties(node);
    if (type == "widget")
        widget = std::unique_ptr<GuiWidget>(new GuiWidget(properties));
    else if (type == "button")
        widget = std::unique_ptr<GuiButton>(new GuiButton(properties));
    else if (type == "text")
        widget = std::unique_ptr<GuiText>(new GuiText(properties));
    else if (type == "image")
        widget = std::unique_ptr<GuiImage>(new GuiImage(properties));
    return std::move(widget);
}

std::unique_ptr<GuiLayout> GuiManager::createLayout(tinyxml2::XMLElement* node)
{
    std::unique_ptr<GuiLayout> layout(nullptr);
    std::string type = node->Name();
    PropertyList properties = createProperties(node);
    if (type == "hboxlayout")
        layout = std::unique_ptr<GuiHBoxLayout>(new GuiHBoxLayout(properties));
    else if (type == "vboxlayout")
        layout = std::unique_ptr<GuiVBoxLayout>(new GuiVBoxLayout(properties));
    return std::move(layout);
}

PropertyList GuiManager::createProperties(XMLElement* node)
{
    PropertyList properties;
    for (const XMLAttribute* attribute = node->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
        properties.add(attribute->Name(), attribute->Value());
    return std::move(properties);
}

sf::Vector2f GuiManager::stringToVector(const std::string& s) const
{
    std::istringstream stream(s);
    std::string x, y;
    stream >> x >> y;
    return sf::Vector2f(std::stof(x), std::stof(y));
}
