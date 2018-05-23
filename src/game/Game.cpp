#include "game/Game.h"
#include "game/GameState.h"
#include "game/GameStateStart.h"
#include "game/GameStateEditor.h"

Game::Game()
{
    // Register mailbox
    mMessageBus.addMailbox(mMailbox);

    // Initialize the input engine
    mInputEngine.setWindow(&mRenderEngine.getWindow());

    // Push dependencies
    Subject::setMessageBus(&mMessageBus);
    Gui::setMessageBus(&mMessageBus);
    Gui::setInputEngine(&mInputEngine);
    GameState::setMessageBus(&mMessageBus);
    GameState::setGameId(mMailbox.getId());
    GameState::setRenderEngine(&mRenderEngine);
    GameState::setInputEngine(&mInputEngine);
    GameState::setTextureManager(&mResourceManager.getTextureManager());
    GameState::setStylesheetManager(&mResourceManager.getStylesheetManager());
    GameState::setGuiManager(&mResourceManager.getGuiManager());
    Car::setTextureManager(&mResourceManager.getTextureManager());

    // Load resources
    mResourceManager.setUp();
    Map::loadTiles(mResourceManager.getTextureManager());

    // Add the start state
    pushState(new GameStateStart());
}

Game::~Game()
{
    while (!mStates.empty())
        popState();

    // Free resources
    mResourceManager.tearDown();

    // Unregister mailbox
    mMessageBus.removeMailbox(mMailbox);
}

void Game::pushState(GameState* state)
{
    mStates.push(state);
}

void Game::popState()
{
    delete mStates.top();
    mStates.pop();
}

void Game::changeState(GameState* state)
{
    if (!mStates.empty())
        popState();
    pushState(state);
}

GameState* Game::peekState()
{
    if (mStates.empty())
        return nullptr;
    return mStates.top();
}

void Game::run()
{
    sf::Clock clock;

    while (mRenderEngine.isWindowOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        GameState* curState = peekState();
        if (curState != nullptr)
        {
            mInputEngine.pollEvents();
            curState->handleMessages();
            curState->update(dt);
            curState->draw(dt);
            mRenderEngine.display();
            handleMessages();
        }
    }
}

void Game::handleMessages()
{
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::NEW_GAME)
        {
            GameStateEditor* state = new GameStateEditor();
            state->newGame();
            pushState(state);
        }
        else if (message.type == MessageType::LOAD_GAME)
        {
            GameStateEditor* state = new GameStateEditor();
            state->loadGame("saves/city");
            pushState(state);
        }
    }
}
