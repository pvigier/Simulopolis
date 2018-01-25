#pragma once

#include <stack>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Tile.h"

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
    std::map<std::string, Tile> mTileAtlas;
    static constexpr unsigned int mTileSize = 8;

    void loadTextures();
    void loadTiles();
};
