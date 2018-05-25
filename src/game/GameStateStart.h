#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"

class Gui;

class GameStateStart : public GameState
{
public:
    GameStateStart(bool resume = false);
    virtual ~GameStateStart();

    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleMessages() override;

private:
    std::unique_ptr<Gui> mGui;

    void createGui(bool resume);
};
