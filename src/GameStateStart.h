#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Gui.h"

class GameStateStart : public GameState
{
public:
    GameStateStart(Game* game);

    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleInput() override;

private:
    sf::View mView;
    GuiSystem mGuiSystem;

    void loadGame();
};
