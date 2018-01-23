#pragma once

#include <stack>
#include <SFML/Graphics.hpp>

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

private:
    sf::RenderWindow mWindow;
    std::stack<GameState*> mStates;
};
