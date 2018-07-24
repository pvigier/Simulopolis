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
    GameState::setAudioEngine(&mAudioEngine);
    GameState::setTextureManager(&mResourceManager.getTextureManager());
    GameState::setStylesheetManager(&mResourceManager.getStylesheetManager());
    GameState::setGuiManager(&mResourceManager.getGuiManager());
    GameState::setMusicManager(&mResourceManager.getMusicManager());
    Car::setTextureManager(&mResourceManager.getTextureManager());
    Car::setImageManager(&mResourceManager.getImageManager());
    Tile::setTextureManager(&mResourceManager.getTextureManager());
    Tile::setImageManager(&mResourceManager.getImageManager());
    Person::setMessageBus(&mMessageBus);
    VMarket::setMessageBus(&mMessageBus);

    // Load resources
    mResourceManager.setUp();
    Map::loadTiles(mResourceManager.getTextureManager());

    // Add musics
    mAudioEngine.addMusic(mResourceManager.getMusicManager().getMusic("gymnopedie1"));
    mAudioEngine.addMusic(mResourceManager.getMusicManager().getMusic("gymnopedie2"));
    mAudioEngine.addMusic(mResourceManager.getMusicManager().getMusic("gymnopedie3"));

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
        if (message.type == MessageType::RESUME_GAME)
            popState();
        if (message.type == MessageType::NEW_GAME)
        {
            GameStateEditor* state = new GameStateEditor();
            state->newGame();
            changeState(state);
        }
        else if (message.type == MessageType::LOAD_GAME)
        {
            GameStateEditor* state = new GameStateEditor();
            state->loadGame("saves/city");
            changeState(state);
        }
        else if (message.type == MessageType::DISPLAY_MENU)
        {
            const sf::Texture& texture = static_cast<GameStateEditor*>(mStates.top())->getCityTexture();
            GameStateStart* state = new GameStateStart(true);
            state->setCityTexture(texture);
            pushState(state);
        }
    }
}
