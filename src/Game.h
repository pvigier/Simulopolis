#pragma once

#include <stack>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Tile.h"
#include "GuiStyle.h"

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
    TileAtlas& getTileAtlas();
    GuiStyle& getStylesheet(const std::string& name);

private:
    sf::RenderWindow mWindow;
    std::stack<GameState*> mStates;
    TextureManager mTextureManager;
    sf::Sprite mBackground;
    TileAtlas mTileAtlas;
    static constexpr unsigned int mTileSize = 8;
    std::map<std::string, sf::Font> mFonts;
    std::map<std::string, GuiStyle> mStylesheets;

    void loadTextures();
    void loadTiles();
    void loadFonts();
    void loadStylesheets();
};
