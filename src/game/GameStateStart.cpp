#include "game/GameStateStart.h"
#include "message/MessageBus.h"
#include "graphics/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"

GameStateStart::GameStateStart() : GameState()
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

void GameStateStart::draw(const float dt)
{
    sRenderEngine->setView(mView);

    sRenderEngine->clear();
    sRenderEngine->draw(mBackground);

    for (auto gui : mGuiSystem)
       sRenderEngine->draw(gui.second);
}

void GameStateStart::update(const float dt)
{

}

void GameStateStart::handleMessages()
{
    sf::Vector2i mousePosition = sInputEngine->getMousePosition();
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type != MessageType::INPUT)
            continue;
        sf::Event event = message.getInfo<sf::Event>();
        switch (event.type)
        {
            case sf::Event::Closed:
                sRenderEngine->getWindow().close();
                break;
            case sf::Event::Resized:
                mView.setSize(event.size.width, event.size.height);
                mBackground.setPosition(sRenderEngine->getWindow().mapPixelToCoords(sf::Vector2i(0, 0)));
                mBackground.setScale(
                    float(event.size.width) / float(mBackground.getTexture()->getSize().x),
                    float(event.size.height) / float(mBackground.getTexture()->getSize().y));
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    sRenderEngine->getWindow().close();
                else if (event.key.code == sf::Keyboard::Space)
                    loadGame();
                break;
            case sf::Event::MouseMoved:
                // Highlight menu items
                mGuiSystem.at("menu").highlight(mGuiSystem.at("menu").getEntry(sRenderEngine->getWindow().mapPixelToCoords(mousePosition, mView)));
                break;
            case sf::Event::MouseButtonPressed:
                // Click on menu items
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    std::string message = mGuiSystem.at("menu").activate(sRenderEngine->getWindow().mapPixelToCoords(mousePosition, mView));
                    if(message == "load_game")
                        loadGame();
                }
                break;
            default:
                break;
        }
    }
}

void GameStateStart::createGui()
{
    mGuiSystem.emplace("menu", Gui(sf::Vector2f(192, 32), 4, false, sStylesheetManager->getStylesheet("button"),
        {std::make_pair("Load Game", "load_game")}));

    sf::Vector2f pos = sf::Vector2f(sRenderEngine->getWindow().getSize()) * 0.5f;
    mGuiSystem.at("menu").setPosition(pos);
    mGuiSystem.at("menu").setOrigin(96, 32 * 0.5f);
    mGuiSystem.at("menu").show();
}

void GameStateStart::loadGame()
{
    sMessageBus->send(Message(UNDEFINED, sGameId, MessageType::PUSH_GAME_STATE,
        std::make_shared<GameStateName>(GameStateName::EDITOR)));
}
