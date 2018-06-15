#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"
#include "city/City.h"
#include "gui/Gui.h"
#include "pcg/TerrainGenerator.h"

class GuiWindow;
class GuiImage;

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
    struct PersonWindow
    {
        const Person* person;
        GuiWindow* window;
        GuiImage* image;
        sf::RenderTexture renderTexture;
    };

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

    void drawCity(sf::RenderTexture& renderTexture, const sf::View& view);

    void createGui();
    void createPersonWindow(const Person& person);
    void createCompanyWindow(const Company& company);
    void updateWindows();
    void closeMenus();

    void zoom(float factor);

    unsigned int getCost(Tile::Type type) const;
    unsigned int computeCostOfSelection() const;
};
