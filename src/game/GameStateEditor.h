#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"
#include "city/City.h"
#include "gui/Gui.h"
#include "pcg/TerrainGenerator.h"

enum class ActionState{NONE, PANNING, SELECTING};

class GameStateEditor : public GameState
{
public:
    GameStateEditor();
    virtual ~GameStateEditor();

    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleMessages() override;

    void newGame();
    void loadGame(const std::string& path);

    const sf::Texture& getCityTexture() const;

private:
    sf::RenderTexture mRenderTexture;
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
    std::unique_ptr<Gui> mGui;
    TerrainGenerator mTerrainGenerator;

    void createGui();
    void createPersonWindow(const Person& person);
    void createCompanyWindow(const Company& company);
    void closeMenus();

    void zoom(float factor);

    unsigned int getCost(Tile::Type type) const;
    unsigned int computeCostOfSelection() const;
};
