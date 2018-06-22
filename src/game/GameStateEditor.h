#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"
#include "city/City.h"
#include "gui/Gui.h"
#include "pcg/TerrainGenerator.h"

class PersonWindow;
class BuildingWindow;

enum class ActionState{NONE, PANNING, SELECTING};

class GameStateEditor : public GameState
{
public:
    GameStateEditor();
    virtual ~GameStateEditor();

    virtual void draw(float dt) override;
    virtual void update(float dt) override;
    virtual void handleMessages() override;

    void newGame();
    void loadGame(const std::string& path);

    const sf::Texture& getCityTexture() const;

private:
    sf::RenderTexture mRenderTexture;
    sf::View mGameView;
    sf::Sprite mBackground;
    City mCity;
    ActionState mActionState;
    sf::Vector2i mPanningAnchor;
    float mZoomLevel;
    sf::Vector2i mSelectionStart;
    sf::Vector2i mSelectionEnd;
    Tile::Type mCurrentTile;
    TerrainGenerator mTerrainGenerator;
    // Gui
    std::unique_ptr<Gui> mGui;
    sf::View mGuiView;
    int mIWindow;
    std::vector<std::unique_ptr<PersonWindow>> mPersonWindows;
    std::vector<std::unique_ptr<BuildingWindow>> mBuildingWindows;

    void drawCity(sf::RenderTexture& renderTexture, const sf::View& view);

    void createGui();
    void createPersonWindow(const Person& person);
    void createBuildingWindow(const Building& building);
    void updateWindows();

    void zoom(float factor);

    unsigned int getCost(Tile::Type type) const;
    unsigned int computeCostOfSelection() const;
};
