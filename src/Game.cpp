#include "Game.h"
#include "GameState.h"

Game::Game()
{
    mWindow.create(sf::VideoMode(800, 600), "City Builder");
    mWindow.setFramerateLimit(60);

    loadTextures();
    loadTiles();
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
            mWindow.clear();
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
