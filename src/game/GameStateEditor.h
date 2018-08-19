#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"
#include "city/City.h"
#include "gui/Gui.h"
#include "pcg/TerrainGenerator.h"
#include "game/WindowManager.h"

class ImmigrantsWindow;
class CitizensWindow;
class RentalMarketWindow;
class LaborMarketWindow;
class PoliciesWindow;

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
    std::vector<WindowManager> mWindowManagers;
    ImmigrantsWindow* mImmigrantsWindow;
    CitizensWindow* mCitizensWindow;
    RentalMarketWindow* mRentalMarketWindow;
    LaborMarketWindow* mLaborMarketWindow;
    PoliciesWindow* mPoliciesWindow;
    std::vector<std::unique_ptr<sf::RenderTexture>> mMenuTextures;

    void drawCity(sf::RenderTexture& renderTexture, const sf::View& view);

    void createGui();
    void stopSelecting();
    void generateMenuTextures();
    void openPersonWindow(const Person& person);
    void openBuildingWindow(const Building& building);
    void openImmigrantsWindow();
    void openCitizensWindow();
    void openRentalMarketWindow();
    void openLaborMarketWindow();
    void openPoliciesWindow();
    void updateWindows();
    bool updateTabs(const std::string& name);
    bool updateTile(const std::string& name);

    void zoom(float factor);

    Money getCost(Tile::Type type) const;
    Money computeCostOfSelection() const;

    Id extractId(const std::string& name, const std::string& prefix) const;

    // Events
    void onNewImmigrant(Person* person);
    void onNewMonth();
    void onNewYear();
};
