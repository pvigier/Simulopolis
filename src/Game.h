#pragma once

#include <stack>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class GameState;

class Game
{
public:
    Game();
    ~Game();

    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();

    void gameLoop();

    sf::RenderWindow& getWindow();
    sf::Sprite& getBackground();

private:
    sf::RenderWindow mWindow;
    std::stack<GameState*> mStates;
    TextureManager mTextureManager;
    sf::Sprite mBackground;

    void loadTextures();
};
