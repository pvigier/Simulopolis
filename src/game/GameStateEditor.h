#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"
#include "city/City.h"
#include "gui/Gui.h"

enum class ActionState{NONE, PANNING, SELECTING};

class GameStateEditor : public GameState
{
public:
    GameStateEditor();
    virtual ~GameStateEditor();

    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleMessages() override;

private:
    sf::View mGameView;
    sf::View mGuiView;
    sf::Sprite mBackground;
    City mCity;
    ActionState mActionState;
    sf::Vector2i mPanningAnchor;
    float mZoomLevel;
    sf::Vector2i mSelectionStart;
    sf::Vector2i mSelectionEnd;
    Tile::Type mCurrentTile;
    Gui* mGui;

    void createGui();

    void zoom(float factor);

    unsigned int getCost(Tile::Type type) const;
    unsigned int computeCostOfSelection() const;
};
