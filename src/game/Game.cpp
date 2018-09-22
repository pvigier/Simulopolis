/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "game/Game.h"
#include "game/GameState.h"
#include "game/GameStateStart.h"
#include "game/GameStateEditor.h"
#include "game/GameStateNewCity.h"
#include "game/GameStateLoadCity.h"
#include "game/GameStateSettings.h"

Game::Game()
{
    // Register mailbox
    mMessageBus.addMailbox(mMailbox);

    // Initialize the input engine
    mInputEngine.setRenderEngine(&mRenderEngine);
    mInputEngine.setSubjectMessageBus(&mMessageBus);

    // Push dependencies
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
    GameState::setSaveManager(&mResourceManager.getSaveManager());
    Car::setTextureManager(&mResourceManager.getTextureManager());
    Car::setImageManager(&mResourceManager.getImageManager());
    Tile::setTextureManager(&mResourceManager.getTextureManager());
    Tile::setImageManager(&mResourceManager.getImageManager());

    // Load resources
    mResourceManager.setUp();
    Map::loadTiles();

    // Set icon
    mRenderEngine.setIcon(mResourceManager.getImageManager().getImage("icon"));

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
                    popState();
                    break;
                case GameState::Event::Type::OPEN_NEW_CITY_SCREEN:
                    pushState(std::make_unique<GameStateNewCity>());
                    break;
                case GameState::Event::Type::OPEN_CITY_LOADING_SCREEN:
                    pushState(std::make_unique<GameStateLoadCity>());
                    break;
                case GameState::Event::Type::OPEN_SETTINGS:
                    pushState(std::make_unique<GameStateSettings>());
                    break;
                case GameState::Event::Type::NEW_GAME:
                {
                    GameStateNewCity* newCityState = static_cast<GameStateNewCity*>(peekState());
                    std::string cityName = newCityState->getCityName();
                    uint64_t seed = newCityState->getSeed();
                    clearStates();
                    std::unique_ptr<GameStateEditor> state = std::make_unique<GameStateEditor>();
                    state->newGame(cityName, seed);
                    pushState(std::move(state));
                    break;
                }
                case GameState::Event::Type::LOAD_GAME:
                {
                    GameStateLoadCity* loadCityState = static_cast<GameStateLoadCity*>(peekState());
                    std::string cityName = loadCityState->getSelectedCity();
                    clearStates();
                    std::unique_ptr<GameStateEditor> state = std::make_unique<GameStateEditor>();
                    state->loadGame(cityName);
                    pushState(std::move(state));
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
