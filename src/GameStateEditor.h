#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "City.h"
#include "gui/Gui.h"

enum class ActionState{NONE, PANNING, SELECTING};

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
    City mCity;
    ActionState mActionState;
    sf::Vector2i mPanningAnchor;
    float mZoomLevel;
    sf::Vector2i mSelectionStart;
    sf::Vector2i mSelectionEnd;
    Tile* mCurrentTile;
    GuiSystem mGuiSystem;

    void createGui();
};
