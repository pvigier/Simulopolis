#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

class StylesheetManager;
class Gui;
class Building;
class GuiWindow;
class GuiImage;

class BuildingWindow
{
public:
    BuildingWindow(Gui* gui, StylesheetManager* stylesheetManager, const std::string& windowId,
        const Building& building);
    ~BuildingWindow();

    GuiWindow* getWindow();
    sf::RenderTexture& getRenderTexture();
    sf::View getView();

private:
    const Building& mBuilding;
    GuiWindow* mWindow;
    GuiImage* mImage;
    sf::RenderTexture mRenderTexture;
};
