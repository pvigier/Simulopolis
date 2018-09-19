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

#include "game/GameStateLoadCity.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEvent.h"
#include "audio/AudioEngine.h"
#include "resource/GuiManager.h"
#include "gui/Gui.h"
#include "gui/GuiButton.h"
#include "gui/GuiEvent.h"
#include "util/format.h"

GameStateLoadCity::GameStateLoadCity() : mGui(sGuiManager->getGui("load_city"))
{
    // Gui
    createGui();
    mGui->subscribe(mMailbox.getId());
}

GameStateLoadCity::~GameStateLoadCity()
{
    mGui->get("loadCityButton")->unsubscribe(mMailbox.getId());
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateLoadCity::enter()
{
    mGui->handleMessages();
    // Subscribe to inputs
    mGui->setListen(true);
}

void GameStateLoadCity::handleMessages()
{
    mGui->handleMessages();
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::INPUT)
        {
            const InputEvent& event = message.getInfo<InputEvent>();
            switch (event.type)
            {
                case sf::Event::Closed:
                    sRenderEngine->closeWindow();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(Event::Type::OPEN_MENU)));
                    break;
                default:
                    break;
            }
        }
        if (message.type == MessageType::GUI)
        {
            const GuiEvent& event = message.getInfo<GuiEvent>();
            switch (event.type)
            {
                case GuiEvent::Type::BUTTON_RELEASED:
                {
                    const std::string& name = event.widget->getName();
                    if (name == "loadCityButton")
                    {
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(Event::Type::LOAD_GAME)));
                    }
                }
                default:
                    break;
            }
        }
    }
}

void GameStateLoadCity::update(float /*dt*/)
{
    sAudioEngine->update();
    mGui->update();
}

void GameStateLoadCity::draw()
{
    sRenderEngine->draw(*mGui);
}

void GameStateLoadCity::exit()
{
    // Unsubscribe to inputs
    mGui->setListen(false);
}

void GameStateLoadCity::createGui()
{
    mGui->setViewportSize(sRenderEngine->getViewportSize());

    // Register to events
    mGui->get("loadCityButton")->subscribe(mMailbox.getId());
}
