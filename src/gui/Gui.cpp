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

#include "gui/Gui.h"
#include "message/MessageBus.h"
#include "input/InputEngine.h"
#include "gui/GuiWidget.h"
#include "gui/GuiWindow.h"
#include "gui/GuiEvent.h"

MessageBus* Gui::sMessageBus = nullptr;
InputEngine* Gui::sInputEngine = nullptr;

Gui::Gui() : mVisible(true), mListen(true), mCounter(0)
{
    // Register the mailbox
    sMessageBus->addMailbox(mMailbox);
    sInputEngine->subscribe(mMailbox.getId());
    setSubjectMessageBus(sMessageBus);
}

Gui::~Gui()
{
    // Remove widgets
    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
        remove(mRootWidgets[i]);

    // Unregister the mailbox
    if (mListen)
        sInputEngine->unsubscribe(mMailbox.getId());
    sMessageBus->removeMailbox(mMailbox);
}

void Gui::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

void Gui::setInputEngine(InputEngine* inputEngine)
{
    sInputEngine = inputEngine;
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!mVisible)
        return;

    target.setView(mView);

    // Draw each button of the menu
    for (GuiWidget* widget : mRootWidgets)
        widget->render(target, states, sf::FloatRect(sf::Vector2f(), mView.getSize()));
}

void Gui::add(const std::string& name, std::unique_ptr<GuiWidget> widget)
{
    if (widget->hasGuiEvents())
        widget->subscribe(mMailbox.getId());
    widget->setGui(this);
    widget->setName(name);
    widget->setRoot(false);
    widget->setSubjectMessageBus(sMessageBus);
    mWidgets[name] = std::move(widget);
    mWidgets[name]->setUp();
}

void Gui::addRoot(const std::string& name, std::unique_ptr<GuiWidget> widget)
{
    if (widget->hasGuiEvents())
        widget->subscribe(mMailbox.getId());
    widget->setGui(this);
    widget->setName(name);
    widget->setRoot(true);
    widget->setSubjectMessageBus(sMessageBus);
    mRootWidgets.push_back(widget.get());
    mWidgets[name] = std::move(widget);
    mWidgets[name]->setUp();
}

void Gui::addWithDefaultName(std::unique_ptr<GuiWidget> widget)
{
    add(generateName(), std::move(widget));
}

void Gui::addRootWithDefaultName(std::unique_ptr<GuiWidget> widget)
{
    addRoot(generateName(), std::move(widget));
}

GuiWidget* Gui::get(const std::string& name)
{
    return mWidgets[name].get();
}

const GuiWidget* Gui::get(const std::string& name) const
{
    return mWidgets.at(name).get();
}

void Gui::remove(const std::string& name)
{
    remove(mWidgets[name].get());
}

void Gui::remove(GuiWidget* widget)
{
    widget->tearDown();
    // Alert the parent
    if (widget->isRoot())
        mRootWidgets.erase(std::find(mRootWidgets.begin(), mRootWidgets.end(), widget));
    else
        widget->getParent()->remove(widget);
    // Remove children
    std::vector<GuiWidget*> children = widget->getChildren(); // Deep copy
    for (GuiWidget* child : children)
        remove(child);
    mWidgets.erase(widget->getName());
}

void Gui::update()
{
    for (GuiWidget* widget : mRootWidgets)
    {
        if (widget->isDirty())
            widget->update();
    }
}

void Gui::handleMessages()
{
    // Poll events
    sf::Vector2f mousePosition(sInputEngine->getMousePosition());
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::INPUT)
        {
            InputEvent& event = message.getInfo<InputEvent>();
            bool processed = false;
            switch (event.type)
            {
                /*case sf::Event::Resized:
                    mView.setSize(event.size.width, event.size.height);
                    break;*/
                case sf::Event::MouseMoved:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseMoved(mousePosition, processed) || processed;
                    break;
                case sf::Event::MouseButtonPressed:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseButtonPressed(mousePosition, processed) || processed;
                    break;
                case sf::Event::MouseButtonReleased:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseButtonReleased(mousePosition, processed) || processed;
                    break;
                case sf::Event::MouseWheelMoved:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseWheelScrolled(event.mouseWheel.delta, processed) || processed;
                    break;
                case sf::Event::TextEntered:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateTextEntered(event.text.unicode, processed) || processed;
                    break;
                case sf::Event::KeyPressed:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateKeyPressed(event.key.code, processed) || processed;
                    break;
                default:
                    break;
            }
            event.processed = processed;
            notify(message);
        }
        else if (message.type == MessageType::GUI)
        {
            const GuiEvent& event = message.getInfo<GuiEvent>();
            switch (event.type)
            {
                case GuiEvent::Type::WINDOW_CLOSED:
                    remove(event.widget);
                    break;
                default:
                    break;
            }
        }
    }
}

const sf::View& Gui::getView()
{
    return mView;
}

void Gui::setViewportSize(sf::Vector2i viewportSize)
{
    mView = sf::View(sf::FloatRect(sf::Vector2f(), sf::Vector2f(viewportSize)));
}

void Gui::setVisible(bool visible)
{
    mVisible = visible;
}

bool Gui::isVisible() const
{
    return mVisible;
}

void Gui::setListen(bool listen)
{
    if (mListen != listen)
    {
        mListen = listen;
        if (mListen)
            sInputEngine->subscribe(mMailbox.getId());
        else
            sInputEngine->unsubscribe(mMailbox.getId());
    }
}

std::string Gui::generateName()
{
    std::string name = std::to_string(mCounter);
    while (mWidgets.find(name) != mWidgets.end())
        name = std::to_string(++mCounter);
    return name;
}
