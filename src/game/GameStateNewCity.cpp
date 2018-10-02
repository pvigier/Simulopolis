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

#include "game/GameStateNewCity.h"
#include <chrono>
#include <limits>
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEvent.h"
#include "audio/AudioEngine.h"
#include "resource/GuiManager.h"
#include "resource/SaveManager.h"
#include "gui/Gui.h"
#include "gui/GuiButton.h"
#include "gui/GuiInput.h"
#include "gui/GuiEvent.h"
#include "pcg/noise.h"
#include "util/format.h"
#include "util/regex.h"

GameStateNewCity::GameStateNewCity() : mGui(sGuiManager->getGui("new_city"))
{
    // Gui
    createGui();
    mGui->subscribe(mMailbox.getId());
}

GameStateNewCity::~GameStateNewCity()
{
    mGui->get("createCityButton")->unsubscribe(mMailbox.getId());
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateNewCity::enter()
{
    mGui->setViewportSize(sRenderEngine->getViewportSize());
    // Subscribe to inputs
    mGui->setListen(true);
}

void GameStateNewCity::handleMessages()
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
                case sf::Event::Resized:
                    mGui->setViewportSize(sf::Vector2u(event.size.width, event.size.height));
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
                    std::string name = event.widget->getName();
                    if (name == "createCityButton")
                    {
                        std::string cityName = getCityName();
                        if (!cityName.empty() && !sSaveManager->hasSave(cityName))
                            sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(Event::Type::NEW_GAME)));
                    }
                }
                default:
                    break;
            }
        }
    }
}

void GameStateNewCity::update(float /*dt*/)
{
    sAudioEngine->update();
    mGui->update();
}

void GameStateNewCity::draw()
{
    sRenderEngine->draw(*mGui);
}

void GameStateNewCity::exit()
{
    // Unsubscribe to inputs
    mGui->setListen(false);
}

uint64_t GameStateNewCity::getSeed() const
{
    return std::stoull(mGui->get<GuiInput>("seedInput")->getString().toAnsiString());
}

std::string GameStateNewCity::getCityName() const
{
    return mGui->get<GuiInput>("nameInput")->getString().toAnsiString();
}

void GameStateNewCity::createGui()
{
    // Seed
    uint64_t seed = integer_noise(std::chrono::system_clock::now().time_since_epoch().count());
    mGui->get<GuiInput>("seedInput")->setString(std::to_string(seed));
    mGui->get<GuiInput>("seedInput")->setRegex(regexNumbersUntil(std::numeric_limits<uint64_t>::max()));

    // Register to events
    mGui->get("createCityButton")->subscribe(mMailbox.getId());
}
