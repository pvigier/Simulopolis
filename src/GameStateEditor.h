#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"

class GameStateEditor : public GameState
{
public:
    GameStateEditor(Game* game);

    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleInput() override;

private:
    sf::View mGameView;
    sf::View mGuiView;
};
