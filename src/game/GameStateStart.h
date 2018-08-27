#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"

class Gui;

class GameStateStart : public GameState
{
public:
    GameStateStart(bool resume = false);
    virtual ~GameStateStart();

    virtual void enter() override;
    virtual void handleMessages() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void exit() override;

    void setCityTexture(const sf::Texture& texture);

private:
    std::unique_ptr<Gui> mGui;

    void createGui(bool resume);
};
