#include "game/GameStateStart.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "resource/XmlDocument.h"
#include "gui/Gui.h"
#include "gui/GuiButton.h"
#include "gui/GuiText.h"

GameStateStart::GameStateStart(bool resume) : mGui(sGuiManager->getGui("menu"))
{
    // Gui
    createGui(resume);

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
            std::string info = message.getInfo<std::string>();
            if (info == "new_game")
                sMessageBus->send(Message::create(sGameId, MessageType::NEW_GAME));
            else if (info == "load_game")
                sMessageBus->send(Message::create(sGameId, MessageType::LOAD_GAME));
            else if (info == "exit")
                sRenderEngine->getWindow().close();
        }
    }
}

void GameStateStart::createGui(bool resume)
{
    mGui->setWindowSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    mGui->get("menu")->setSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));

    // Resume button
    const PropertyList& textProperties = sStylesheetManager->getStylesheet("button")->getFirstChildByName("text").getAttributes();
    sf::Color textColor = textProperties.get<sf::Color>("color");
    if (resume)
        mGui->get<GuiButton>("resumeGameButton")->setState(GuiButton::State::NORMAL);
    else
    {
        mGui->get<GuiButton>("resumeGameButton")->setState(GuiButton::State::DISABLED);
        textColor = textProperties.get<sf::Color>("disableColor", textColor);
    }
    mGui->get<GuiText>("resumeGameText")->setColor(textColor);

    // Register to events
    mGui->get("resumeGameButton")->subscribe(mMailbox.getId());
    mGui->get("newGameButton")->subscribe(mMailbox.getId());
    mGui->get("loadGameButton")->subscribe(mMailbox.getId());
    mGui->get("exitButton")->subscribe(mMailbox.getId());
}
