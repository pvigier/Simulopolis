#include "resource/GuiManager.h"
#include <iostream>
#include <sstream>
#include "resource/XmlManager.h"
#include "resource/PropertyList.h"
#include "gui/GuiWidget.h"
#include "gui/GuiButton.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiHBoxLayout.h"
#include "gui/GuiVBoxLayout.h"

GuiManager::GuiManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

GuiManager::~GuiManager()
{
    //dtor
}

void GuiManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void GuiManager::setUp()
{
    std::string path = mPrefixPath + "guis.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
        loadGui(child);
}

void GuiManager::tearDown()
{

}

void GuiManager::addGui(const std::string& name, XmlDocument gui)
{
    mGuis[name] = std::make_unique<XmlDocument>(std::move(gui));
}

std::unique_ptr<Gui> GuiManager::getGui(const std::string& name)
{
    std::unique_ptr<Gui> gui(new Gui());
    loadRootWidgets(gui.get(), *mGuis[name]);
    return std::move(gui);
}

void GuiManager::loadGui(const XmlDocument& node)
{
    // Name and path
    std::string name = node.getAttributes().get("name");
    std::string path = mPrefixPath + node.getAttributes().get("path");

    mGuis[name] = std::make_unique<XmlDocument>(mXmlManager->loadDocument(path));
}

void GuiManager::loadRootWidgets(Gui* gui, const XmlDocument& node)
{
    for (const XmlDocument& child : node.getChildren())
        gui->addRoot(child.getAttributes().get("name"), loadWidgets(gui, child));
}

std::unique_ptr<GuiWidget> GuiManager::loadWidgets(Gui* gui, const XmlDocument& node)
{
    // Create widget
    std::unique_ptr<GuiWidget> widget = createWidget(node);
    // Create children
    for (const XmlDocument& child : node.getChildren())
    {
        if (isWidget(child))
        {
            std::unique_ptr<GuiWidget> childWidget = loadWidgets(gui, child);
            widget->add(childWidget.get());
            gui->add(child.getAttributes().get("name"), std::move(childWidget));
        }
        else if (isLayout(child))
            widget->setLayout(createLayout(child));
    }
    return widget;
}

bool GuiManager::isWidget(const XmlDocument& node)
{
    const std::string& type = node.getName();
    return (type == "widget" || type == "button" || type == "text" || type == "image");
}

bool GuiManager::isLayout(const XmlDocument& node)
{
    const std::string& type = node.getName();
    return (type == "hboxlayout" || type == "vboxlayout");
}

std::unique_ptr<GuiWidget> GuiManager::createWidget(const XmlDocument& node)
{
    std::unique_ptr<GuiWidget> widget;
    const std::string& type = node.getName();
    if (type == "widget")
        widget = std::unique_ptr<GuiWidget>(new GuiWidget(node.getAttributes()));
    else if (type == "button")
        widget = std::unique_ptr<GuiButton>(new GuiButton(node.getAttributes()));
    else if (type == "text")
        widget = std::unique_ptr<GuiText>(new GuiText(node.getAttributes()));
    else if (type == "image")
        widget = std::unique_ptr<GuiImage>(new GuiImage(node.getAttributes()));
    return std::move(widget);
}

std::unique_ptr<GuiLayout> GuiManager::createLayout(const XmlDocument& node)
{
    std::unique_ptr<GuiLayout> layout(nullptr);
    const std::string& type = node.getName();
    if (type == "hboxlayout")
        layout = std::unique_ptr<GuiHBoxLayout>(new GuiHBoxLayout(node.getAttributes()));
    else if (type == "vboxlayout")
        layout = std::unique_ptr<GuiVBoxLayout>(new GuiVBoxLayout(node.getAttributes()));
    return std::move(layout);
}
