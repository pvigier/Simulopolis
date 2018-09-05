#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"

class Gui;

class GameStateNewCity : public GameState
{
public:
    GameStateNewCity();
    virtual ~GameStateNewCity();

    virtual void enter() override;
    virtual void handleMessages() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void exit() override;

private:
    std::unique_ptr<Gui> mGui;

    void createGui();
};
