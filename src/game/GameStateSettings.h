#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"

class Gui;

class GameStateSettings : public GameState
{
public:
    GameStateSettings();
    virtual ~GameStateSettings();

    virtual void enter() override;
    virtual void handleMessages() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void exit() override;

private:
    std::unique_ptr<Gui> mGui;
    // tmp
    bool mFullscreen;
    bool mMute;

    void createGui();
};
