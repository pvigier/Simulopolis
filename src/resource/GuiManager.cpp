#include "resource/GuiManager.h"
#include <iostream>
#include <sstream>
#include "resource/TextureManager.h"
#include "resource/FontManager.h"
#include "resource/StylesheetManager.h"
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

GuiWidgetPtr GuiManager::loadWidget(Gui* gui, XMLElement* node)
{
    // Create widget
    GuiWidgetPtr widget = createWidget(node);
    // Create children
    for (XMLElement* child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (isWidget(child))
        {
            GuiWidgetPtr childWidget = loadWidget(gui, child);
            widget->add(childWidget);
            gui->add(child->Attribute("name"), childWidget);
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

GuiWidgetPtr GuiManager::createWidget(tinyxml2::XMLElement* node)
{
    GuiWidgetPtr widget(nullptr);
    std::string type = node->Name();
    if (type == "widget")
        widget = GuiWidgetPtr(new GuiWidget());
    else if (type == "button")
    {
        sf::Vector2f dimensions = stringToVector(node->Attribute("dimensions"));
        Message message = Message::create<std::string>(MessageType::GUI, node->Attribute("message"));
        const GuiStyle& style = mStylesheetManager->getStylesheet(node->Attribute("style"));
        widget = std::make_shared<GuiButton>(dimensions, message, style);
    }
    else if (type == "text")
    {
        std::string text = node->Attribute("text");
        unsigned int characterSize = node->IntAttribute("characterSize");
        const GuiStyle& style = mStylesheetManager->getStylesheet(node->Attribute("style"));
        widget = std::make_shared<GuiText>(text, characterSize, style);
    }
    else if (type == "image")
    {
        const sf::Texture& texture = mTextureManager->getTexture(node->Attribute("texture"));
        widget = std::make_shared<GuiImage>(texture);
    }
    return std::move(widget);
}

GuiLayoutPtr GuiManager::createLayout(tinyxml2::XMLElement* node)
{
    GuiLayoutPtr layout(nullptr);
    std::string type = node->Name();
    if (type == "hboxlayout")
    {
        GuiLayout::HAlignment hAlignment = stringToHAlignment(node->Attribute("hAlignment"));
        GuiLayout::VAlignment vAlignment = stringToVAlignment(node->Attribute("vAlignment"));
        float spacing = node->FloatAttribute("spacing");
        std::unique_ptr<GuiHBoxLayout> hboxlayout(new GuiHBoxLayout(hAlignment, vAlignment));
        hboxlayout->setSpacing(spacing);
        layout = std::move(hboxlayout);
    }
    else if (type == "vboxlayout")
    {
        GuiLayout::HAlignment hAlignment = stringToHAlignment(node->Attribute("hAlignment"));
        GuiLayout::VAlignment vAlignment = stringToVAlignment(node->Attribute("vAlignment"));
        float spacing = node->FloatAttribute("spacing");
        std::unique_ptr<GuiVBoxLayout> vboxlayout(new GuiVBoxLayout(hAlignment, vAlignment));
        vboxlayout->setSpacing(spacing);
        layout = std::move(vboxlayout);
    }
    return std::move(layout);
}

sf::Vector2f GuiManager::stringToVector(const std::string& s) const
{
    std::istringstream stream(s);
    std::string x, y;
    stream >> x >> y;
    return sf::Vector2f(std::stof(x), std::stof(y));
}

GuiLayout::HAlignment GuiManager::stringToHAlignment(const std::string& s) const
{
    if (s == "left")
        return GuiLayout::HAlignment::Left;
    else if (s == "center")
        return GuiLayout::HAlignment::Center;
    else
        return GuiLayout::HAlignment::Right;
}

GuiLayout::VAlignment GuiManager::stringToVAlignment(const std::string& s) const
{
    if (s == "top")
        return GuiLayout::VAlignment::Top;
    else if (s == "center")
        return GuiLayout::VAlignment::Center;
    else
        return GuiLayout::VAlignment::Bottom;
}
