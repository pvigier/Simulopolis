#include "game/GameStateStart.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "gui/Gui.h"
#include "gui/GuiButton.h"
#include "gui/GuiImage.h"
#include "gui/GuiText.h"
#include "gui/GuiVBoxLayout.h"

GameStateStart::GameStateStart() : mGui(sGuiManager->getGui("menu"))
{
    // Gui
    createGui();

    // Subscribe to inputs
    mGui->subscribe(mMailbox.getId());
}

GameStateStart::~GameStateStart()
{
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateStart::draw(const float dt)
{
    sRenderEngine->draw(*mGui);
}

void GameStateStart::update(const float dt)
{
    mGui->update();
}

void GameStateStart::handleMessages()
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
                    sRenderEngine->getWindow().close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        sRenderEngine->getWindow().close();
                    else if (event.key.code == sf::Keyboard::Space)
                        loadGame();
                    break;
                default:
                    break;
            }
        }
        if (message.type == MessageType::GUI)
        {
            std::string info = message.getInfo<std::string>();
            if (info == "load_game")
                loadGame();
            else if (info == "exit")
                sRenderEngine->getWindow().close();
        }
    }
}

void GameStateStart::createGui()
{
    mGui->setWindowSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    mGui->get("menu")->setSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));

    // Register to events
    mGui->get("loadGameButton")->subscribe(mMailbox.getId());
    mGui->get("exitButton")->subscribe(mMailbox.getId());
}

void GameStateStart::loadGame()
{
    sMessageBus->send(Message::create(sGameId, MessageType::PUSH_GAME_STATE, GameStateName::EDITOR));
}
