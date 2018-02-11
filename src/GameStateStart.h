#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "gui/Gui.h"

class GameStateStart : public GameState
{
public:
    GameStateStart();

    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleMessages() override;

private:
    sf::View mView;
    sf::Sprite mBackground;
    GuiSystem mGuiSystem;

    void createGui();
    void loadGame();
};
