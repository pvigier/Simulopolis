#include "game/GameStateLoadCity.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
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
}

GameStateLoadCity::~GameStateLoadCity()
{
    mGui->get("loadCityButton")->unsubscribe(mMailbox.getId());
}

void GameStateLoadCity::enter()
{
    mGui->handleMessages();
    // Subscribe to inputs
    mGui->subscribe(mMailbox.getId());
}

void GameStateLoadCity::handleMessages()
{
    mGui->handleMessages();
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::INPUT)
        {
            sf::Event event = message.getInfo<sf::Event>();
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
            GuiEvent event = message.getInfo<GuiEvent>();
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

void GameStateLoadCity::update(float dt)
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
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateLoadCity::createGui()
{
    mGui->setViewportSize(sRenderEngine->getViewportSize());

    // Register to events
    mGui->get("loadCityButton")->subscribe(mMailbox.getId());
}
