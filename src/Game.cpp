#include "Game.h"
#include "GameState.h"

Game::Game()
{
    mWindow.create(sf::VideoMode(800, 600), "City Builder");
    mWindow.setFramerateLimit(60);

    loadTextures();
    loadTiles();
    loadFonts();
    loadStylesheets();
    mBackground.setTexture(mTextureManager.getRef("background"));
}

Game::~Game()
{
    while (!mStates.empty())
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

void Game::gameLoop()
{
    sf::Clock clock;

    while (mWindow.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        GameState* curState = peekState();
        if (curState != nullptr)
        {
            curState->handleInput();
            curState->update(dt);
            curState->draw(dt);
            mWindow.display();
        }
    }
}

sf::RenderWindow& Game::getWindow()
{
    return mWindow;
}

sf::Sprite& Game::getBackground()
{
    return mBackground;
}

TileAtlas& Game::getTileAtlas()
{
    return mTileAtlas;
}

GuiStyle& Game::getStylesheet(const std::string name)
{
    return mStylesheets[name];
}

void Game::loadTextures()
{
    mTextureManager.loadTexture("background", "media/background.png");

    mTextureManager.loadTexture("grass", "media/grass.png");
    mTextureManager.loadTexture("forest", "media/forest.png");
    mTextureManager.loadTexture("water", "media/water.png");
    mTextureManager.loadTexture("residential", "media/residential.png");
    mTextureManager.loadTexture("commercial", "media/commercial.png");
    mTextureManager.loadTexture("industrial", "media/industrial.png");
    mTextureManager.loadTexture("road", "media/road.png");
}

void Game::loadTiles()
{
    Animation staticAnim(0, 0, 1.0f);

    mTileAtlas["grass"] = Tile(mTileSize, 1, mTextureManager.getRef("grass"),
        {staticAnim},
        TileType::GRASS, 50, 0, 1);

    mTileAtlas["forest"] = Tile(mTileSize, 1, mTextureManager.getRef("forest"),
        {staticAnim},
        TileType::FOREST, 100, 0, 1);

    mTileAtlas["water"] = Tile(mTileSize, 1, mTextureManager.getRef("water"),
        {Animation(0, 3, 0.5f), Animation(0, 3, 0.5f), Animation(0, 3, 0.5f)},
        TileType::WATER, 0, 0, 1);

    mTileAtlas["residential"] = Tile(mTileSize, 2, mTextureManager.getRef("residential"),
        {staticAnim, staticAnim, staticAnim, staticAnim, staticAnim, staticAnim},
        TileType::RESIDENTIAL, 300, 50, 6);

    mTileAtlas["commercial"] = Tile(mTileSize, 2, mTextureManager.getRef("commercial"),
        {staticAnim, staticAnim, staticAnim, staticAnim},
        TileType::COMMERCIAL, 300, 50, 4);

    mTileAtlas["industrial"] = Tile(mTileSize, 2, mTextureManager.getRef("industrial"),
        {staticAnim, staticAnim, staticAnim, staticAnim},
        TileType::INDUSTRIAL, 300, 50, 4);

    mTileAtlas["road"] = Tile(mTileSize, 1, mTextureManager.getRef("road"),
        {staticAnim, staticAnim, staticAnim, staticAnim, staticAnim, staticAnim,
        staticAnim, staticAnim, staticAnim, staticAnim, staticAnim},
        TileType::ROAD, 100, 0, 1);
}

void Game::loadFonts()
{
    sf::Font font;
    font.loadFromFile("media/font.ttf");
    mFonts["main_font"] = font;
}

void Game::loadStylesheets()
{
    /*GuiStyle(sf::Font* font, float borderSize,
        sf::Color bodyCol, sf::Color borderCol, sf::Color textCol,
        sf::Color bodyHighlightCol, sf::Color borderHighlightCol, sf::Color textHighlightCol)

    mStylesheets["button"] = GuiStyle(&mFonts.at("main_font"), 1,
        sf::Color(0xc6,0xc6,0xc6), sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00),
        sf::Color(0x61,0x61,0x61), sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00));
    mStylesheets["text"] = GuiStyle(&mFonts.at("main_font"), 0,
        sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00), sf::Color(0xff,0xff,0xff),
        sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00), sf::Color(0xff,0x00,0x00));

    GuiStyle(sf::Color bodyColor, sf::Color bodyHighlightColor,
        sf::Color borderColor, sf::Color borderHighlightColor, float borderSize,
        sf::Color textColor, sf::Color textHighlightColor, sf::Font* font)*/

    mStylesheets["button"] = GuiStyle(sf::Color(0xc6, 0xc6, 0xc6), sf::Color(0x61, 0x61, 0x61),
        sf::Color(0x94, 0x94, 0x94), sf::Color(0x94,0x94,0x94), 1,
        sf::Color(0x00, 0x00, 0x00), sf::Color(0x00,0x00,0x00), &mFonts.at("main_font"));
    mStylesheets["text"] = GuiStyle(sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00, 0x00),
        sf::Color(0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), 0,
        sf::Color(0xff, 0xff, 0xff), sf::Color(0xff, 0x00, 0x00), &mFonts.at("main_font"));



}
