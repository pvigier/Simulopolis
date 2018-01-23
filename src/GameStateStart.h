#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"

class GameStateStart : public GameState
{
public:
    GameStateStart(Game* game);

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

private:
    sf::View mView;

    void loadGame();
};
