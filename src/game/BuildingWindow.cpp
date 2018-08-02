#include "BuildingWindow.h"
#include "resource/StylesheetManager.h"
#include "city/Building.h"
#include "city/Company.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "util/format.h"

BuildingWindow::BuildingWindow(StylesheetManager* stylesheetManager, const Building& building) :
    GuiWindow(format("%s %d", Tile::typeToString(building.getType()).c_str(), building.getId()), stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mBuilding(building), mImage(nullptr)
{

}

BuildingWindow::~BuildingWindow()
{
    //dtor
}

void BuildingWindow::setUp()
{
    // Zoom
    mRenderTexture.create(96, 96);
    sf::Sprite sprite(mRenderTexture.getTexture());
    mImage = mGui->createWithDefaultName<GuiImage>(sprite);

    // Personal info
    auto infoWidget = mGui->createWithDefaultName<GuiWidget>();
    auto typeText = mGui->createWithDefaultName<GuiText>("Type: " + Tile::typeToString(mBuilding.getType()), 12, mStylesheetManager->getStylesheet("button"));
    auto ownerText = mGui->createWithDefaultName<GuiText>("Owner: " + mBuilding.getOwner()->getName(), 12, mStylesheetManager->getStylesheet("button"));
    infoWidget->add(typeText);
    infoWidget->add(ownerText);
    infoWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Top widget
    auto topWidget = mGui->createWithDefaultName<GuiWidget>();
    topWidget->add(mImage);
    topWidget->add(infoWidget);
    topWidget->setLayout(std::make_unique<GuiHBoxLayout>(8.0f));

    // Bottom widget
    auto bottomWidget = mGui->createWithDefaultName<GuiWidget>();
    bottomWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Window
    add(topWidget);
    add(bottomWidget);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
}

sf::RenderTexture& BuildingWindow::getRenderTexture()
{
    return mRenderTexture;
}

sf::View BuildingWindow::getView()
{
    sf::Vector2f center = mBuilding.getPosition() + sf::Vector2f(66.0f, 48.0f);
    return sf::View(center, 2.0f * sf::Vector2f(mRenderTexture.getSize()));
}
