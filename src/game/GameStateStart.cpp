#include "game/GameStateStart.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "gui/GuiButton.h"
#include "gui/GuiImage.h"
#include "gui/GuiText.h"
#include "gui/GuiVBoxLayout.h"

GameStateStart::GameStateStart() :
    mGui()
{
    // Gui
    mGui.setWindowSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
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
            else if (info == "exit")
                sRenderEngine->getWindow().close();
        }
    }
}

void GameStateStart::createGui()
{
    // Background
    auto background = mGui.createRoot<GuiImage>("background", sTextureManager->getTexture("background"));

    // Load button
    auto loadGameButton = mGui.create<GuiButton>("loadGameButton", sf::Vector2f(192, 32),
        Message::create<std::string>(MessageType::GUI, "load_game"), sStylesheetManager->getStylesheet("button"));
    auto loadGameText = mGui.create<GuiText>("loadGameText", "Load Game", 24, sStylesheetManager->getStylesheet("button"));
    loadGameButton->add(loadGameText);
    loadGameButton->setLayout(GuiLayoutPtr(new GuiVBoxLayout(GuiLayout::HAlignment::Center, GuiLayout::VAlignment::Center)));

    // Exit button
    auto exitButton = mGui.create<GuiButton>("exitButton", sf::Vector2f(192, 32),
        Message::create<std::string>(MessageType::GUI, "exit"), sStylesheetManager->getStylesheet("button"));
    auto exitText = mGui.create<GuiText>("exitText", "Exit", 24, sStylesheetManager->getStylesheet("button"));
    exitButton->add(exitText);
    exitButton->setLayout(GuiLayoutPtr(new GuiVBoxLayout(GuiLayout::HAlignment::Center, GuiLayout::VAlignment::Center)));

    auto menu = mGui.createRoot<GuiWidget>("menu");
    menu->setSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    menu->add(loadGameButton);
    menu->add(exitButton);
    std::unique_ptr<GuiVBoxLayout> menuLayout(new GuiVBoxLayout(GuiLayout::HAlignment::Center, GuiLayout::VAlignment::Center));
    menuLayout->setSpacing(16.0f);
    menu->setLayout(std::move(menuLayout));

    // Register to events
    loadGameButton->subscribe(mMailbox.getId());
    exitButton->subscribe(mMailbox.getId());
}

void GameStateStart::loadGame()
{
    sMessageBus->send(Message::create(sGameId, MessageType::PUSH_GAME_STATE, GameStateName::EDITOR));
}
