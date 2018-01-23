#pragma once

#include "Game.h"

class GameState
{
public:
    GameState();
    virtual ~GameState();

    virtual void draw(const float dt) = 0;
    virtual void update(const float dt) = 0;
    virtual void handleInput() = 0;

protected:
    Game* mGame;
};
