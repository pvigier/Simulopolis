#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Map.h"

enum class ActionState { NONE, PANNING };

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
    Map mMap;
    ActionState mActionState;
    sf::Vector2i mPanningAnchor;
    float mZoomLevel;
};
