/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.h"
#include "city/City.h"
#include "gui/Gui.h"
#include "game/WindowManager.h"

class ImmigrantsWindow;
class CitizensWindow;
class RentalMarketWindow;
class LaborMarketWindow;
class GoodsMarketWindow;
class PoliciesWindow;

enum class ActionState{NONE, PANNING, SELECTING};

class GameStateEditor : public GameState
{
public:
    GameStateEditor();
    virtual ~GameStateEditor();

    virtual void enter() override;
    virtual void handleMessages() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void exit() override;

    void newGame(std::string cityName, uint64_t seed);
    void loadGame(const std::string& cityName);

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
    // Gui
    std::unique_ptr<Gui> mGui;
    std::vector<WindowManager> mWindowManagers;
    ImmigrantsWindow* mImmigrantsWindow;
    CitizensWindow* mCitizensWindow;
    RentalMarketWindow* mRentalMarketWindow;
    LaborMarketWindow* mLaborMarketWindow;
    GoodsMarketWindow* mGoodsMarketWindow;
    PoliciesWindow* mPoliciesWindow;
    std::vector<std::unique_ptr<sf::RenderTexture>> mMenuTextures;

    void drawCity(sf::RenderTexture& renderTexture, const sf::View& view, bool background);
    void generatePreview(sf::Vector2u size, sf::Texture& texture);

    // Gui
    void createGui();
    void generateMenuTextures();
    void openPersonWindow(const Person& person);
    void openBuildingWindow(const Building& building);
    void openImmigrantsWindow();
    void openCitizensWindow();
    void openRentalMarketWindow();
    void openLaborMarketWindow();
    void openGoodsMarketWindow();
    void openPoliciesWindow();
    void updateWindows();
    bool updateTabs(const std::string& name);
    bool updateTile(const std::string& name);

    // Editor
    void startPanning(sf::Vector2i mousePosition);
    void pan(sf::Vector2i mousePosition);
    void startSelecting(sf::Vector2f gamePosition);
    void stopSelecting();
    void zoom(float factor);

    Money getCost(Tile::Type type) const;
    Money computeCostOfSelection() const;

    Id extractId(const std::string& name, const std::string& prefix) const;

    // Events
    void onNewYear();
};
