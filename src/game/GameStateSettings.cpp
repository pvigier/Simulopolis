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

#include "game/GameStateSettings.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEvent.h"
#include "audio/AudioEngine.h"
#include "resource/GuiManager.h"
#include "gui/Gui.h"
#include "gui/GuiButton.h"
#include "gui/GuiLabel.h"
#include "gui/GuiImage.h"
#include "gui/GuiEvent.h"
#include "util/format.h"

GameStateSettings::GameStateSettings() : mGui(sGuiManager->getGui("settings")),
    mFullscreen(false), mMute(false)
{
    // Gui
    createGui();
    mGui->subscribe(mMailbox.getId());
}

GameStateSettings::~GameStateSettings()
{
    mGui->get("fullscreenButton")->unsubscribe(mMailbox.getId());
    mGui->get("musicButton")->unsubscribe(mMailbox.getId());
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateSettings::enter()
{
    mGui->setViewportSize(sRenderEngine->getViewportSize());
    // Subscribe to inputs
    mGui->setListen(true);
}

void GameStateSettings::handleMessages()
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
                    const std::string& name = event.widget->getName();
                    if (name == "fullscreenButton")
                    {
                        mFullscreen = !mFullscreen;
                        if (mFullscreen)
                            mGui->get<GuiLabel>("fullscreenText")->setString("Fullscreen: on");
                        else
                            mGui->get<GuiLabel>("fullscreenText")->setString("Fullscreen: off");
                        sRenderEngine->setFullscreen(mFullscreen);
                    }
                    else if (name == "musicButton")
                    {
                        mMute = !mMute;
                        if (mMute)
                            mGui->get<GuiLabel>("musicText")->setString("Music: off");
                        else
                            mGui->get<GuiLabel>("musicText")->setString("Music: on");
                        sAudioEngine->setVolume(!mMute * 100.0f);
                    }
                }
                default:
                    break;
            }
        }
    }
}

void GameStateSettings::update(float /*dt*/)
{
    sAudioEngine->update();
    mGui->update();
}

void GameStateSettings::draw()
{
    sRenderEngine->draw(*mGui);
}

void GameStateSettings::exit()
{
    // Unsubscribe to inputs
    mGui->setListen(false);
}

void GameStateSettings::setCityTexture(const sf::Texture& texture)
{
    mGui->get<GuiImage>("city")->setSprite(sf::Sprite(texture));
}

void GameStateSettings::createGui()
{
    // Register to events
    mGui->get("fullscreenButton")->subscribe(mMailbox.getId());
    mGui->get("musicButton")->subscribe(mMailbox.getId());
}
