#include "game/Game.h"
#include "game/GameState.h"
#include "game/GameStateStart.h"
#include "game/GameStateEditor.h"
#include "game/GameStateNewCity.h"
#include "game/GameStateLoadCity.h"

Game::Game()
{
    // Register mailbox
    mMessageBus.addMailbox(mMailbox);

    // Initialize the input engine
    mInputEngine.setRenderEngine(&mRenderEngine);

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
    Company::setMessageBus(&mMessageBus);
    Bank::setMessageBus(&mMessageBus);

    // Load resources
    mResourceManager.setUp();
    Map::loadTiles();

    // Add musics
    mAudioEngine.addMusic(mResourceManager.getMusicManager().getMusic("gymnopedie1"));
    mAudioEngine.addMusic(mResourceManager.getMusicManager().getMusic("gymnopedie2"));
    mAudioEngine.addMusic(mResourceManager.getMusicManager().getMusic("gymnopedie3"));

    // Add the start state
    pushState(std::make_unique<GameStateStart>());
}

Game::~Game()
{
    // Free resources
    mResourceManager.tearDown();

    // Unregister mailbox
    mMessageBus.removeMailbox(mMailbox);
}

void Game::pushState(std::unique_ptr<GameState> state)
{
    if (!mStates.empty())
        mStates.top()->exit();
    mStates.push(std::move(state));
    mStates.top()->enter();
}

void Game::popState()
{
    mStates.top()->exit();
    mStates.pop();
    if (!mStates.empty())
        mStates.top()->enter();
}

void Game::changeState(std::unique_ptr<GameState> state)
{
    if (!mStates.empty())
    {
        mStates.top()->exit();
        mStates.pop();
    }
    mStates.push(std::move(state));
    mStates.top()->enter();
}

GameState* Game::peekState()
{
    if (mStates.empty())
        return nullptr;
    return mStates.top().get();
}

void Game::clearStates()
{
    while (!mStates.empty())
        mStates.pop();
}

void Game::run()
{
    sf::Clock clock;

    while (mRenderEngine.isWindowOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        GameState* curState = peekState();
        if (curState)
        {
            mInputEngine.pollEvents();
            curState->handleMessages();
            curState->update(dt);
            mRenderEngine.clear();
            curState->draw();
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
        if (message.type == MessageType::GAME)
        {
            const GameState::Event& event = message.getInfo<GameState::Event>();
            switch (event.type)
            {
                case GameState::Event::Type::OPEN_MENU:
                    clearStates();
                    pushState(std::make_unique<GameStateStart>(false));
                    break;
                case GameState::Event::Type::OPEN_NEW_CITY_SCREEN:
                    changeState(std::make_unique<GameStateNewCity>());
                    break;
                case GameState::Event::Type::OPEN_CITY_LOADING_SCREEN:
                    changeState(std::make_unique<GameStateLoadCity>());
                    break;
                case GameState::Event::Type::NEW_GAME:
                {
                    std::unique_ptr<GameStateEditor> state = std::make_unique<GameStateEditor>();
                    state->newGame(event.seed);
                    changeState(std::move(state));
                    break;
                }
                case GameState::Event::Type::LOAD_GAME:
                {
                    std::unique_ptr<GameStateEditor> state = std::make_unique<GameStateEditor>();
                    state->loadGame("saves/city");
                    changeState(std::move(state));
                    break;
                }
                case GameState::Event::Type::PAUSE_GAME:
                {
                    const sf::Texture& texture = static_cast<GameStateEditor*>(peekState())->getCityTexture();
                    std::unique_ptr<GameStateStart> state = std::make_unique<GameStateStart>(true);
                    state->setCityTexture(texture);
                    pushState(std::move(state));
                    break;
                }
                case GameState::Event::Type::RESUME_GAME:
                    popState();
                    break;
                default:
                    break;
            }
        }
    }
}
