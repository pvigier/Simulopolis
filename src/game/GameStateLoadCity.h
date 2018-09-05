#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"

class Gui;

class GameStateLoadCity : public GameState
{
public:
    GameStateLoadCity();
    virtual ~GameStateLoadCity();

    virtual void enter() override;
    virtual void handleMessages() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void exit() override;

private:
    std::unique_ptr<Gui> mGui;

    void createGui();
};
