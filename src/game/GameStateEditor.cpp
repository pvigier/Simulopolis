#include "game/GameStateEditor.h"
#include <utility>
#include <chrono>
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "audio/AudioEngine.h"
#include "message/MessageBus.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "gui/GuiButton.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiWindow.h"
#include "gui/GuiTabWidget.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "gui/GuiEvent.h"
#include "game/PersonWindow.h"
#include "game/BuildingWindow.h"

GameStateEditor::GameStateEditor() : mActionState(ActionState::NONE), mZoomLevel(1.0f),
    mCurrentTile(Tile::Type::GRASS), mGui(sGuiManager->getGui("editor")), mIWindow(0)
{
    // Views
    sf::Vector2f windowSize = sf::Vector2f(sRenderEngine->getWindow().getSize());
    mGuiView.setSize(windowSize);
    mGameView.setSize(windowSize);
    mGuiView.setCenter(windowSize * 0.5f);

    // Render texture
    mRenderTexture.create(windowSize.x, windowSize.y);

    // Background
    mBackground.setTexture(sTextureManager->getTexture("background"));

    // Gui
    createGui();

    // Subscribe to inputs
    mGui->subscribe(mMailbox.getId());
}

GameStateEditor::~GameStateEditor()
{
    mGui->get<GuiButton>("landscapeTabButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("housingTabButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("industryTabButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("businessTabButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("healthTabButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("securityTabButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("educationTabButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadTabButton")->unsubscribe(mMailbox.getId());
    //mGui->get<GuiButton>("roadGrassButton")->unsubscribe(mMailbox.getId());
    //mGui->get<GuiButton>("roadSidewalkButton")->unsubscribe(mMailbox.getId());
    //mGui->get<GuiButton>("roadWaterButton")->unsubscribe(mMailbox.getId());
    mGui->unsubscribe(mMailbox.getId());

    for (std::unique_ptr<PersonWindow>& personWindow : mPersonWindows)
        personWindow->getWindow()->unsubscribe(mMailbox.getId());
    for (std::unique_ptr<BuildingWindow>& buildingWindow : mBuildingWindows)
        buildingWindow->getWindow()->unsubscribe(mMailbox.getId());
}

void GameStateEditor::draw(float dt)
{
    sRenderEngine->clear();

    // City
    drawCity(mRenderTexture, mGameView);

    // GUI
    sRenderEngine->setView(mGuiView);
    sRenderEngine->draw(sf::Sprite(mRenderTexture.getTexture()));
    sRenderEngine->draw(*mGui);
}

void GameStateEditor::update(float dt)
{
    sAudioEngine->update();
    mCity.update(dt);

    // Update the info bar at the bottom of the screen
    mGui->get<GuiText>("dayText")->setText("Day: " + std::to_string(mCity.getDay()));
    mGui->get<GuiText>("fundsText")->setText("$" + std::to_string(mCity.getFunds()));
    mGui->get<GuiText>("populationText")->setText("Population: " + std::to_string(mCity.getPopulation()));
    mGui->get<GuiText>("employmentText")->setText("Unemployment: " + std::to_string(mCity.getUnemployed()));
    mGui->get<GuiText>("currentTileText")->setText(Tile::typeToString(mCurrentTile));

    // Update the windows
    updateWindows();

    mGui->update();
}

void GameStateEditor::handleMessages()
{
    mGui->handleMessages();

    sf::Vector2i mousePosition = sInputEngine->getMousePosition();
    sf::Vector2f gamePos = sRenderEngine->getWindow().mapPixelToCoords(mousePosition, mGameView);
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::INPUT)
        {
            sf::Event event = message.getInfo<sf::Event>();
            switch (event.type)
            {
                case sf::Event::Closed:
                    sRenderEngine->getWindow().close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        sMessageBus->send(Message::create(sGameId, MessageType::DISPLAY_MENU));
                    break;
                case sf::Event::MouseMoved:
                    // Pan the camera
                    if (mActionState == ActionState::PANNING)
                    {
                        sf::Vector2f delta(mousePosition - mPanningAnchor);
                        mGameView.move(-1.0f * delta * mZoomLevel);
                        mPanningAnchor = mousePosition;
                    }
                    // Select tiles
                    else if(mActionState == ActionState::SELECTING)
                    {
                        mSelectionEnd = mCity.toTileIndices(gamePos);
                        mCity.getMap().deselect();
                        if (mCurrentTile == Tile::Type::GRASS)
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, static_cast<Tile::Category>(0));
                        else if (mCurrentTile == Tile::Type::ROAD_WATER)
                        {
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, Tile::Category::ROAD | Tile::Category::WATER);
                        }
                        else
                        {
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, Tile::Category::GROUND);
                        }
                        // Update the GUI
                        unsigned int totalCost = computeCostOfSelection();
                        auto selectionCostText = mGui->get<GuiText>("selectionCostText");
                        selectionCostText->setText("$" + std::to_string(totalCost));
                        //selectionCostText->setHighlight(mCity.getFunds() < totalCost);
                        selectionCostText->setPosition(sf::Vector2f(mousePosition) + sf::Vector2f(16, -16));
                        selectionCostText->setVisible(true);
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    // Start panning
                    if (event.mouseButton.button == sf::Mouse::Middle)
                    {
                        mGui->get("selectionCostText")->setVisible(false);
                        if (mActionState != ActionState::PANNING)
                        {
                            mActionState = ActionState::PANNING;
                            mPanningAnchor = sf::Mouse::getPosition(sRenderEngine->getWindow());
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        // Stop selecting
                        if (mActionState == ActionState::SELECTING)
                        {
                            mActionState = ActionState::NONE;
                            mCity.getMap().deselect();
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        // Select map tile
                        if (mActionState != ActionState::SELECTING)
                        {
                            mActionState = ActionState::SELECTING;
                            mSelectionStart = mCity.toTileIndices(gamePos);
                        }
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    // Stop panning
                    if (event.mouseButton.button == sf::Mouse::Middle)
                        mActionState = ActionState::NONE;
                    // Stop selecting
                    else if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (mActionState == ActionState::SELECTING)
                        {
                            unsigned int totalCost = computeCostOfSelection();
                            if(mCity.getFunds() >= totalCost)
                            {
                                mCity.bulldoze(mCurrentTile);
                                mCity.decreaseFunds(totalCost);
                            }
                            mGui->get("selectionCostText")->setVisible(false);
                            mActionState = ActionState::NONE;
                            mCity.getMap().deselect();
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        City::Intersection intersection = mCity.intersect(gamePos);
                        if (intersection.type == City::Intersection::Type::CAR)
                            createPersonWindow(*intersection.car->getOwner());
                        else if (intersection.type == City::Intersection::Type::BUILDING)
                            createBuildingWindow(*intersection.building);
                    }
                    break;
                case sf::Event::MouseWheelMoved:
                    // Zoom the view
                    if (event.mouseWheel.delta < 0)
                        zoom(2.0f);
                    else
                        zoom(0.5f);
                    break;
                default:
                    break;
            }
        }
        if (message.type == MessageType::GUI)
        {
            GuiEvent event = message.getInfo<GuiEvent>();
            switch (event.type)
            {
                case GuiEvent::Type::BUTTON_RELEASED:
                {
                    const std::string& name = event.widget->getName();
                    if (!updateTabs(name))
                        updateTile(name);
                }
                case GuiEvent::Type::WINDOW_CLOSED:
                {
                    GuiWindow* window = static_cast<GuiWindow*>(event.widget);
                    for (std::size_t i = 0; i < mPersonWindows.size(); ++i)
                    {
                        if (mPersonWindows[i]->getWindow() == window)
                        {
                            std::swap(mPersonWindows[i], mPersonWindows.back());
                            mPersonWindows.pop_back();
                            break;
                        }
                    }
                    for (std::size_t i = 0; i < mBuildingWindows.size(); ++i)
                    {
                        if (mBuildingWindows[i]->getWindow() == window)
                        {
                            std::swap(mBuildingWindows[i], mBuildingWindows.back());
                            mBuildingWindows.pop_back();
                            break;
                        }
                    }
                }
                default:
                    break;
            }
        }
    }
}

void GameStateEditor::newGame()
{
    mCity.createMap(mTerrainGenerator.generate(std::chrono::system_clock::now().time_since_epoch().count()));
    mGameView.setCenter(sf::Vector2f(mCity.getMap().getWidth() * Tile::SIZE,
        mCity.getMap().getHeight() * Tile::SIZE * 0.5f));
    zoom(8.0f);
}

void GameStateEditor::loadGame(const std::string& path)
{
    mCity.load(path);
    mGameView.setCenter(sf::Vector2f(mCity.getMap().getWidth() * Tile::SIZE,
        mCity.getMap().getHeight() * Tile::SIZE * 0.5f));
    zoom(8.0f);
}

const sf::Texture& GameStateEditor::getCityTexture() const
{
    return mRenderTexture.getTexture();
}

void GameStateEditor::drawCity(sf::RenderTexture& renderTexture, const sf::View& view)
{
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.setView(mGuiView);
    renderTexture.draw(mBackground);
    renderTexture.setView(view);
    renderTexture.draw(mCity);
    renderTexture.display();
}

void GameStateEditor::createGui()
{
    mGui->setWindowSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    mGui->get("infoBar")->setFixedSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));

    mGui->get<GuiButton>("landscapeTabButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("housingTabButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("industryTabButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("businessTabButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("healthTabButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("securityTabButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("educationTabButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadTabButton")->subscribe(mMailbox.getId());
    //mGui->get<GuiButton>("roadGrassButton")->subscribe(mMailbox.getId());
    //mGui->get<GuiButton>("roadSidewalkButton")->subscribe(mMailbox.getId());
    //mGui->get<GuiButton>("roadWaterButton")->subscribe(mMailbox.getId());
    mGui->get("rightMenu")->setFixedSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
}

void GameStateEditor::createPersonWindow(const Person& person)
{
    std::string windowId = "window" + std::to_string(mIWindow++);
    PersonWindow* personWindow = new PersonWindow(mGui.get(), sStylesheetManager, windowId, person, mCity.getYear());
    personWindow->getWindow()->subscribe(mMailbox.getId());
    mPersonWindows.emplace_back(personWindow);
}

void GameStateEditor::createBuildingWindow(const Building& building)
{
    std::string windowId = "window" + std::to_string(mIWindow++);
    BuildingWindow* buildingWindow = new BuildingWindow(mGui.get(), sStylesheetManager, windowId, building);
    buildingWindow->getWindow()->subscribe(mMailbox.getId());
    mBuildingWindows.emplace_back(buildingWindow);
}

void GameStateEditor::updateWindows()
{
    for (std::unique_ptr<PersonWindow>& personWindow : mPersonWindows)
        drawCity(personWindow->getRenderTexture(), personWindow->getView());
    for (std::unique_ptr<BuildingWindow>& buildingWindow : mBuildingWindows)
        drawCity(buildingWindow->getRenderTexture(), buildingWindow->getView());
}

bool GameStateEditor::updateTabs(const std::string& name)
{
    int tab = -1;
    GuiWidget* buttonsWidget = mGui->get("tabButtons");
    for (std::size_t i = 0; i < buttonsWidget->getChildren().size(); ++i)
    {
        if (buttonsWidget->getChildren()[i]->getName() == name)
        {
            tab = i;
            break;
        }
    }
    if (tab == -1)
        return false;
    mGui->get<GuiTabWidget>("tabs")->setCurrentTab(tab);
    for (GuiWidget* widget : buttonsWidget->getChildren())
        static_cast<GuiButton*>(widget)->setState(GuiButton::State::NORMAL);
    mGui->get<GuiButton>(name)->setState(GuiButton::State::FORCE_PRESSED);
    return true;
}

bool GameStateEditor::updateTile(const std::string& name)
{
    mCurrentTile = Tile::stringToType(name.substr(0, name.size() - 6));
    return true;
}

void GameStateEditor::zoom(float factor)
{
    mGameView.zoom(factor);
    mZoomLevel *= factor;
}

unsigned int GameStateEditor::getCost(Tile::Type type) const
{
    switch (type)
    {
        case Tile::Type::GRASS: return 50;
        case Tile::Type::FOREST: return 100;
        /*case Tile::Type::RESIDENTIAL: return 300;
        case Tile::Type::COMMERCIAL: return 300;
        case Tile::Type::INDUSTRIAL: return 300;
        case Tile::Type::ROAD_GRASS: return 100;*/
        default: return 0;
    }
}

unsigned int GameStateEditor::computeCostOfSelection() const
{
    return getCost(mCurrentTile) * mCity.getMap().getNbSelected();
}
