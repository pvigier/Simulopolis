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

#include "game/GameStateStart.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEvent.h"
#include "audio/AudioEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "resource/XmlDocument.h"
#include "gui/Gui.h"
#include "gui/GuiButton.h"
#include "gui/GuiLabel.h"
#include "gui/GuiImage.h"
#include "gui/GuiEvent.h"

GameStateStart::GameStateStart(bool resume) : mGui(sGuiManager->getGui("menu")), mCityTexture(nullptr)
{
    // Gui
    createGui(resume);
    mGui->subscribe(mMailbox.getId());
}

GameStateStart::~GameStateStart()
{
    mGui->get("resumeGameButton")->unsubscribe(mMailbox.getId());
    mGui->get("newGameButton")->unsubscribe(mMailbox.getId());
    mGui->get("loadGameButton")->unsubscribe(mMailbox.getId());
    mGui->get("settingsButton")->unsubscribe(mMailbox.getId());
    mGui->get("exitButton")->unsubscribe(mMailbox.getId());
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateStart::enter()
{
    mGui->setViewportSize(sRenderEngine->getViewportSize());
    // Subscribe to inputs
    mGui->setListen(true);
}

void GameStateStart::handleMessages()
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
                /*case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        sRenderEngine->getWindow().close();
                    break;*/
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
                    if (name == "resumeGameButton")
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(Event::Type::RESUME_GAME)));
                    else if (name == "newGameButton")
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(Event::Type::OPEN_NEW_CITY_SCREEN)));
                    else if (name == "loadGameButton")
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(Event::Type::OPEN_CITY_LOADING_SCREEN)));
                    else if (name == "settingsButton")
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(Event::Type::OPEN_SETTINGS)));
                    else if (name == "exitButton")
                        sRenderEngine->closeWindow();
                }
                default:
                    break;
            }
        }
    }
}

void GameStateStart::update(float /*dt*/)
{
    sAudioEngine->update();
    mGui->update();
}

void GameStateStart::draw()
{
    sRenderEngine->draw(*mGui);
}

void GameStateStart::exit()
{
    // Unsubscribe to inputs
    mGui->setListen(false);
}

const sf::Texture* GameStateStart::getCityTexture() const
{
    return mCityTexture;
}

void GameStateStart::setCityTexture(const sf::Texture& texture)
{
    mGui->get<GuiImage>("city")->setSprite(sf::Sprite(texture));
    mCityTexture = &texture;
}

void GameStateStart::createGui(bool resume)
{
    // Resume button
    const PropertyList& textProperties = sStylesheetManager->getStylesheet("darkText")->getFirstChildByName("text").getAttributes();
    sf::Color textColor = textProperties.get<sf::Color>("color");
    if (resume)
        mGui->get<GuiButton>("resumeGameButton")->setState(GuiButton::State::NORMAL);
    else
    {
        mGui->get<GuiButton>("resumeGameButton")->setState(GuiButton::State::DISABLED);
        textColor = textProperties.get<sf::Color>("disableColor", textColor);
    }
    mGui->get<GuiLabel>("resumeGameText")->setColor(textColor);

    // Register to events
    mGui->get("resumeGameButton")->subscribe(mMailbox.getId());
    mGui->get("newGameButton")->subscribe(mMailbox.getId());
    mGui->get("loadGameButton")->subscribe(mMailbox.getId());
    mGui->get("settingsButton")->subscribe(mMailbox.getId());
    mGui->get("exitButton")->subscribe(mMailbox.getId());
}
