#include "BuildingWindow.h"
#include "resource/StylesheetManager.h"
#include "city/Building.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiWindow.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"

BuildingWindow::BuildingWindow(Gui* gui, StylesheetManager* stylesheetManager, const std::string& windowId,
    const Building& building) :
    mBuilding(building), mWindow(nullptr), mImage(nullptr)
{
    // Zoom
    mRenderTexture.create(96, 96);
    sf::Sprite sprite(mRenderTexture.getTexture());
    mImage = gui->create<GuiImage>(windowId + "ZoomImage", sprite);

    // Personal info
    auto infoWidget = gui->create<GuiWidget>(windowId + "InfoWidget");
    auto typeText = gui->create<GuiText>(windowId + "TypeText", "Type: " + Tile::typeToString(building.getType()), 10, stylesheetManager->getStylesheet("text"));
    auto ownerText = gui->create<GuiText>(windowId + "OwnerText", "Owner: " + building.getOwner().toString(), 10, stylesheetManager->getStylesheet("text"));
    infoWidget->add(typeText);
    infoWidget->add(ownerText);
    infoWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Top widget
    auto topWidget = gui->create<GuiWidget>(windowId + "TopWidget");
    topWidget->add(mImage);
    topWidget->add(infoWidget);
    topWidget->setLayout(std::make_unique<GuiHBoxLayout>(8.0f));

    // Bottom widget
    auto bottomWidget = gui->create<GuiWidget>(windowId + "BottomWidget");
    bottomWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Window
    mWindow = gui->createRoot<GuiWindow>(windowId, "Building", stylesheetManager->getStylesheet("window"));
    mWindow->add(topWidget);
    mWindow->add(bottomWidget);
    mWindow->setPosition(sf::Vector2f(50.0f, 50.0f));
    mWindow->setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
}

BuildingWindow::~BuildingWindow()
{
    //dtor
}

GuiWindow* BuildingWindow::getWindow()
{
    return mWindow;
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
