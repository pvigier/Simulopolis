#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"
#include "gui/Gui.h"

class GameStateStart : public GameState
{
public:
    GameStateStart();
    virtual ~GameStateStart();

    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleMessages() override;

private:
    Gui mGui;

    void createGui();
    void loadGame();
};
