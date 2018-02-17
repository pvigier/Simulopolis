#include "game/GameStateStart.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "gui/GuiButton.h"
#include "gui/GuiVBoxLayout.h"

GameStateStart::GameStateStart() :
    mGui(sf::Vector2f(sRenderEngine->getWindow().getSize()))
{
    // View
    sf::Vector2f pos = sf::Vector2f(sRenderEngine->getWindow().getSize());
    mView.setSize(pos);
    pos *= 0.5f;
    mView.setCenter(pos);

    // Background
    mBackground.setTexture(sTextureManager->getTexture("background"));

    // Gui
    createGui();

    // Subscribe to inputs
    sInputEngine->subscribe(mMailbox.getId());
}

GameStateStart::~GameStateStart()
{
    sInputEngine->unsubscribe(mMailbox.getId());
}

void GameStateStart::draw(const float dt)
{
    sRenderEngine->setView(mView);

    sRenderEngine->clear();
    sRenderEngine->draw(mBackground);

    sRenderEngine->draw(mGui);
}

void GameStateStart::update(const float dt)
{

}

void GameStateStart::handleMessages()
{
    mGui.update();
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
        }
    }
}

void GameStateStart::createGui()
{
    GuiButton* loadGameButton = new GuiButton(sStylesheetManager->getStylesheet("button"),
        "Load Game", sf::Vector2f(192, 32), 24, "load_game");
    loadGameButton->setPosition(sf::Vector2f(sRenderEngine->getWindow().getSize()) * 0.5f);
    mGui.add("loadGameButton", loadGameButton);

    GuiVBoxLayout* menu = new GuiVBoxLayout();
    menu->setSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    menu->setHAlignment(GuiLayout::HAlignment::Center);
    menu->setVAlignment(GuiLayout::VAlignment::Center);
    menu->add(loadGameButton);
    mGui.addRoot("menu", menu);

    // Register to events
    loadGameButton->subscribe(mMailbox.getId());
}

void GameStateStart::loadGame()
{
    sMessageBus->send(Message(UNDEFINED, sGameId, MessageType::PUSH_GAME_STATE,
        std::make_shared<GameStateName>(GameStateName::EDITOR)));
}
