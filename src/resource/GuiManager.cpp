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
 
#include "resource/GuiManager.h"
#include "util/debug.h"
#include "resource/XmlManager.h"
#include "resource/PropertyList.h"
#include "gui/GuiWidget.h"
#include "gui/GuiButton.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiTabWidget.h"
#include "gui/GuiInput.h"
#include "gui/GuiScrollArea.h"
#include "gui/GuiHBoxLayout.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiOverlapLayout.h"

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
    DEBUG_IF(mGuis.find(name) == mGuis.end(), name << " is an invalid gui name.\n");
    std::unique_ptr<Gui> gui = std::make_unique<Gui>();
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
    {
        if (child.getAttributes().has("name"))
            gui->addRoot(child.getAttributes().get("name"), loadWidgets(gui, child));
        else
            gui->addRootWithDefaultName(loadWidgets(gui, child));
    }
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
            if (child.getAttributes().has("name"))
                gui->add(child.getAttributes().get("name"), std::move(childWidget));
            else
                gui->addWithDefaultName(std::move(childWidget));
        }
        else if (isLayout(child))
            widget->setLayout(createLayout(child));
    }
    return widget;
}

bool GuiManager::isWidget(const XmlDocument& node)
{
    const std::string& type = node.getName();
    return (type == "widget" || type == "button" || type == "text" || type == "image" || type == "tabwidget" ||
            type == "input" || type == "scrollarea");
}

bool GuiManager::isLayout(const XmlDocument& node)
{
    const std::string& type = node.getName();
    return (type == "hboxlayout" || type == "vboxlayout" || type == "overlaplayout");
}

std::unique_ptr<GuiWidget> GuiManager::createWidget(const XmlDocument& node)
{
    std::unique_ptr<GuiWidget> widget;
    const std::string& type = node.getName();
    if (type == "widget")
        widget = std::make_unique<GuiWidget>(node.getAttributes());
    else if (type == "button")
        widget = std::make_unique<GuiButton>(node.getAttributes());
    else if (type == "text")
        widget = std::make_unique<GuiText>(node.getAttributes());
    else if (type == "image")
        widget = std::make_unique<GuiImage>(node.getAttributes());
    else if (type == "tabwidget")
        widget = std::make_unique<GuiTabWidget>(node.getAttributes());
    else if (type == "input")
        widget = std::make_unique<GuiInput>(node.getAttributes());
    else if (type == "scrollarea")
        widget = std::make_unique<GuiScrollArea>(node.getAttributes());
    return std::move(widget);
}

std::unique_ptr<GuiLayout> GuiManager::createLayout(const XmlDocument& node)
{
    std::unique_ptr<GuiLayout> layout(nullptr);
    const std::string& type = node.getName();
    if (type == "hboxlayout")
        layout = std::make_unique<GuiHBoxLayout>(node.getAttributes());
    else if (type == "vboxlayout")
        layout = std::make_unique<GuiVBoxLayout>(node.getAttributes());
    else if (type == "overlaplayout")
        layout = std::make_unique<GuiOverlapLayout>(node.getAttributes());
    return std::move(layout);
}
