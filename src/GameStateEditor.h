#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"

class GameStateEditor : public GameState
{
public:
    GameStateEditor(Game* game);

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

private:
    sf::View mGameView;
    sf::View mGuiView;
};
