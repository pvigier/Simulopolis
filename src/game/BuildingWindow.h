#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include "gui/GuiWindow.h"

class StylesheetManager;
class Building;
class GuiImage;
class GuiText;
class GuiTable;

class BuildingWindow : public GuiWindow
{
public:
    BuildingWindow(StylesheetManager* stylesheetManager, const Building& building);
    ~BuildingWindow();

    virtual void setUp() override;

    void update();

    const Building& getBuilding() const;
    sf::RenderTexture& getRenderTexture();
    sf::View getView();

private:
    StylesheetManager* mStylesheetManager;
    const Building& mBuilding;
    GuiImage* mImage;
    GuiTable* mTable;
    sf::RenderTexture mRenderTexture;
};
