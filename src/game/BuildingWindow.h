#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include "gui/GuiWindow.h"

class StylesheetManager;
class Gui;
class Building;
class GuiImage;

class BuildingWindow : public GuiWindow
{
public:
    BuildingWindow(StylesheetManager* stylesheetManager, const Building& building);
    ~BuildingWindow();

    virtual void setUp() override;

    sf::RenderTexture& getRenderTexture();
    sf::View getView();

private:
    StylesheetManager* mStylesheetManager;
    const Building& mBuilding;
    GuiImage* mImage;
    sf::RenderTexture mRenderTexture;
};
