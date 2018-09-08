#include "game/GameStateNewCity.h"
#include <chrono>
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEvent.h"
#include "audio/AudioEngine.h"
#include "resource/GuiManager.h"
#include "gui/Gui.h"
#include "gui/GuiButton.h"
#include "gui/GuiInput.h"
#include "gui/GuiEvent.h"
#include "pcg/noise.h"
#include "util/format.h"

GameStateNewCity::GameStateNewCity() : mGui(sGuiManager->getGui("new_city"))
{
    // Gui
    createGui();
}

GameStateNewCity::~GameStateNewCity()
{
    mGui->get("createCityButton")->unsubscribe(mMailbox.getId());
}

void GameStateNewCity::enter()
{
    mGui->handleMessages();
    // Subscribe to inputs
    mGui->subscribe(mMailbox.getId());
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
                    if (name == "createCityButton")
                    {
                        uint64_t seed = std::stoull(mGui->get<GuiInput>("seedInput")->getString().toAnsiString());
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event(seed)));
                    }
                }
                default:
                    break;
            }
        }
    }
}

void GameStateNewCity::update(float dt)
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
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateNewCity::createGui()
{
    mGui->setViewportSize(sRenderEngine->getViewportSize());

    // Seed
    uint64_t seed = integer_noise(std::chrono::system_clock::now().time_since_epoch().count());
    mGui->get<GuiInput>("seedInput")->setString(std::to_string(seed));

    // Register to events
    mGui->get("createCityButton")->subscribe(mMailbox.getId());
}
