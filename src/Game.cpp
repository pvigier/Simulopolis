#include "Game.h"
#include "GameState.h"

Game::Game()
{
    mWindow.create(sf::VideoMode(800, 600), "City Builder");
    mWindow.setFramerateLimit(60);
}

Game::~Game()
{
    while(!mStates.empty())
        popState();
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
    if(!mStates.empty())
        popState();
    pushState(state);
}

GameState* Game::peekState()
{
    if(mStates.empty())
        return nullptr;
    return mStates.top();
}

void Game::gameLoop()
{
    sf::Clock clock;

    while(mWindow.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        GameState* curState = peekState();
        if(curState != nullptr)
        {
            curState->handleInput();
            curState->update(dt);
            mWindow.clear();
            curState->draw(dt);
            mWindow.display();
        }
    }
}
