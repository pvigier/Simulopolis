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

#include "game/GameStateEditor.h"
#include <utility>
#include "util/format.h"
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "audio/AudioEngine.h"
#include "message/MessageBus.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "resource/SaveManager.h"
#include "gui/GuiButton.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiTabWidget.h"
#include "gui/GuiEvent.h"
#include "game/PersonWindow.h"
#include "game/BuildingWindow.h"
#include "game/ImmigrantsWindow.h"
#include "game/CitizensWindow.h"
#include "game/RentalMarketWindow.h"
#include "game/LaborMarketWindow.h"
#include "game/GoodsMarketWindow.h"
#include "game/PoliciesWindow.h"

GameStateEditor::GameStateEditor() :
    mActionState(ActionState::NONE), mZoomLevel(1.0f),
    mCurrentTile(Tile::Type::GRASS), mGui(sGuiManager->getGui("editor")),
    mImmigrantsWindow(nullptr), mCitizensWindow(nullptr),
    mRentalMarketWindow(nullptr), mLaborMarketWindow(nullptr), mGoodsMarketWindow(nullptr),
    mPoliciesWindow(nullptr)
{
    // Views
    sf::Vector2u viewportSize = sRenderEngine->getViewportSize();
    mGameView.setSize(sf::Vector2f(viewportSize));

    // Render texture
    mRenderTexture.create(viewportSize.x, viewportSize.y);

    // Background
    mBackground.setTexture(sTextureManager->getTexture("background"));

    // Gui
    createGui();
    mGui->subscribe(mMailbox.getId());
}

GameStateEditor::~GameStateEditor()
{
    // Save city
    sf::Texture preview;
    generatePreview(sf::Vector2u(64, 64), preview);
    sSaveManager->save(mCity, preview);
    // Tab buttons
    GuiWidget* buttonsWidget = mGui->get("tabButtons");
    for (GuiWidget* widget : buttonsWidget->getChildren())
        widget->unsubscribe(mMailbox.getId());
    // Tile buttons
    GuiWidget* tabWidget = mGui->get<GuiTabWidget>("tabs");
    for (GuiWidget* widget : tabWidget->getChildren())
    {
        buttonsWidget = widget->getChildren()[1];
        for (GuiWidget* button : buttonsWidget->getChildren())
            button->unsubscribe(mMailbox.getId());
    }
    mGui->unsubscribe(mMailbox.getId());
}

void GameStateEditor::enter()
{
    mGui->setViewportSize(sRenderEngine->getViewportSize());
    // Subscribe to inputs
    mGui->setListen(true);
}

void GameStateEditor::handleMessages()
{
    mGui->handleMessages();

    sf::Vector2i mousePosition = sInputEngine->getMousePosition();
    sf::Vector2f gamePosition = sRenderEngine->mapPixelToCoords(mousePosition, mGameView);
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::INPUT)
        {
            const InputEvent& event = message.getInfo<InputEvent>();
            switch (event.type)
            {
                case sf::Event::Closed:
                    sRenderEngine->closeWindow();
                    break;
                case sf::Event::Resized:
                    mRenderTexture.create(event.size.width, event.size.height);
                    mGameView = sf::View(mGameView.getCenter(), sf::Vector2f(event.size.width, event.size.height));
                    mGameView.zoom(mZoomLevel);
                    mGui->setViewportSize(sf::Vector2u(event.size.width, event.size.height));
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        sMessageBus->send(Message::create(sGameId, MessageType::GAME, Event{Event::Type::PAUSE_GAME}));
                    else if (event.key.code == sf::Keyboard::H)
                        mGui->setVisible(!mGui->isVisible());
                    else if (event.key.code == sf::Keyboard::S)
                        mRenderTexture.getTexture().copyToImage().saveToFile("screenshot.png");
                    else if (event.key.code == sf::Keyboard::LControl &&
                        sInputEngine->isButtonPressed(sf::Mouse::Button::Left))
                        startPanning(mousePosition);
                    break;
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::LControl)
                        mActionState = ActionState::NONE;
                    break;
                case sf::Event::MouseMoved:
                    // Pan the camera
                    if (mActionState == ActionState::PANNING)
                        pan(mousePosition);
                    // Select tiles
                    else if(mActionState == ActionState::SELECTING)
                    {
                        mSelectionEnd = mCity.toTileIndices(gamePosition);
                        mCity.getMap().deselect();
                        if (mCurrentTile == Tile::Type::GRASS)
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, ~(Tile::Category::WATER | Tile::Category::BRIDGE));
                        else if (mCurrentTile == Tile::Type::BRIDGE)
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, Tile::Category::WATER);
                        else if (mCurrentTile == Tile::Type::WATER)
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, Tile::Category::BRIDGE);
                        else
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, Tile::Category::GROUND);
                        // Update the GUI
                        Money totalCost = computeCostOfSelection();
                        GuiText* selectionCostText = mGui->get<GuiText>("selectionCostText");
                        selectionCostText->setString(format("$%.2f", totalCost));
                        if (mCity.getFunds() < totalCost)
                            selectionCostText->setColor(sf::Color::Red);
                        else
                            selectionCostText->setColor(sf::Color::White);
                        selectionCostText->setOutsidePosition(sf::Vector2f(mousePosition) + sf::Vector2f(16, -16));
                        selectionCostText->setVisible(true);
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    // Start panning
                    if (!event.processed && event.mouseButton.button == sf::Mouse::Middle)
                    {
                        if (mActionState != ActionState::PANNING)
                            startPanning(mousePosition);
                        stopSelecting();
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        // Stop selecting and panning
                        mActionState = ActionState::NONE;
                        stopSelecting();
                    }
                    else if (!event.processed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        // Panning
                        if (mActionState != ActionState::PANNING &&
                            sInputEngine->isKeyPressed(sf::Keyboard::LControl))
                            startPanning(mousePosition);
                        // Select map tile
                        else if (mActionState != ActionState::SELECTING)
                            startSelecting(gamePosition);
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    // Stop selecting
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (mActionState == ActionState::SELECTING)
                        {
                            Money totalCost = computeCostOfSelection();
                            if(mCity.getFunds() >= totalCost)
                            {
                                mCity.bulldoze(mCurrentTile);
                                mCity.decreaseFunds(totalCost);
                            }
                        }
                    }
                    else if (!event.processed && event.mouseButton.button == sf::Mouse::Right)
                    {
                        City::Intersection intersection = mCity.intersect(gamePosition);
                        if (intersection.type == City::Intersection::Type::CAR)
                            openPersonWindow(*intersection.car->getDriver());
                        else if (intersection.type == City::Intersection::Type::BUILDING)
                            openBuildingWindow(*intersection.building);
                    }
                    // Stop panning
                    mActionState = ActionState::NONE;
                    stopSelecting();
                    break;
                case sf::Event::MouseWheelMoved:
                    if (!event.processed)
                    {
                        // Zoom the view
                        if (event.mouseWheel.delta < 0)
                            zoom(2.0f);
                        else
                            zoom(0.5f);
                    }
                    break;
                default:
                    break;
            }
        }
        else if (message.type == MessageType::GUI)
        {
            const GuiEvent& event = message.getInfo<GuiEvent>();
            switch (event.type)
            {
                case GuiEvent::Type::BUTTON_RELEASED:
                {
                    const std::string& name = event.widget->getName();
                    if (name == "openImmigrantsWindowButton")
                        openImmigrantsWindow();
                    else if (name == "openCitizensWindowButton")
                        openCitizensWindow();
                    else if (name == "openRentalMarketWindowButton")
                        openRentalMarketWindow();
                    else if (name == "openLaborMarketWindowButton")
                        openLaborMarketWindow();
                    else if (name == "openGoodsMarketWindowButton")
                        openGoodsMarketWindow();
                    else if (name == "openPoliciesWindowButton")
                        openPoliciesWindow();
                    else if (name.substr(0, 16) == "openPersonWindow")
                        openPersonWindow(*mCity.getPerson(extractId(name, "openPersonWindow")));
                    else if (name.substr(0, 16) == "openBuildingWindow")
                        openBuildingWindow(*mCity.getBuilding(extractId(name, "openBuildingWindow")));
                    else if (name == "welcomeAllButton")
                        mCity.welcomeAll();
                    else if (name == "ejectAllButton")
                        mCity.ejectAll();
                    else if (name.substr(0, 8) == "accepted")
                    {
                        Person* person = mCity.getPerson(extractId(name, "accepted"));
                        mCity.welcome(person);
                    }
                    else if (name.substr(0, 8) == "rejected")
                    {
                        Person* person = mCity.getPerson(extractId(name, "rejected"));
                        mCity.eject(person);
                    }
                    else if (!updateTabs(name))
                        updateTile(name);
                    break;
                }
                case GuiEvent::Type::WINDOW_CLOSED:
                {
                    GuiWindow* window = static_cast<GuiWindow*>(event.widget);
                    if (mImmigrantsWindow == window)
                        mImmigrantsWindow = nullptr;
                    else if (mCitizensWindow == window)
                        mCitizensWindow = nullptr;
                    else if (mRentalMarketWindow == window)
                        mRentalMarketWindow = nullptr;
                    else if (mLaborMarketWindow == window)
                        mLaborMarketWindow = nullptr;
                    else if (mGoodsMarketWindow == window)
                        mGoodsMarketWindow = nullptr;
                    else if (mPoliciesWindow == window)
                        mPoliciesWindow = nullptr;
                    else
                    {
                        for (WindowManager& windowManager : mWindowManagers)
                        {
                            if (windowManager.removeWindow(window))
                                break;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
        else if (message.type == MessageType::CITY)
        {
            const City::Event& event = message.getInfo<City::Event>();
            switch (event.type)
            {
                case City::Event::Type::NEW_YEAR:
                    onNewYear();
                    break;
                case City::Event::Type::BUILDING_DESTROYED:
                {
                    std::vector<GuiWindow*>& windows = mWindowManagers[1].getWindows();
                    for (std::size_t i = 0; i < windows.size(); ++i)
                    {
                        if (&static_cast<BuildingWindow*>(windows[i])->getBuilding() == event.building)
                        {
                            mGui->remove(windows[i]);
                            mWindowManagers[1].removeWindow(i);
                            break;
                        }
                    }
                    break;
                }
                case City::Event::Type::CITIZEN_LEFT:
                {
                    std::vector<GuiWindow*>& windows = mWindowManagers[0].getWindows();
                    for (std::size_t i = 0; i < windows.size(); ++i)
                    {
                        if (&static_cast<PersonWindow*>(windows[i])->getPerson() == event.person)
                        {
                            mGui->remove(windows[i]);
                            mWindowManagers[0].removeWindow(i);
                            break;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void GameStateEditor::update(float dt)
{
    sAudioEngine->update();
    mCity.update(dt);

    // Update the info bar at the bottom of the screen
    mGui->get<GuiText>("dateText")->setString(format("%s %d", mCity.getFormattedMonth().c_str(), 2000 + mCity.getYear()));
    mGui->get<GuiText>("fundsText")->setString(format("$%.2f", mCity.getFunds()));
    mGui->get<GuiText>("populationText")->setString(format("Population: %d", mCity.getPopulation()));
    mGui->get<GuiText>("happinessText")->setString(format("Happiness: %.0f", 100.0f * mCity.getAverageHappiness()));
    mGui->get<GuiText>("currentTileText")->setString(Tile::typeToString(mCurrentTile));

    // Update the windows
    updateWindows();

    mGui->update();
}

void GameStateEditor::draw()
{
    // City
    drawCity(mRenderTexture, mGameView, true);

    // GUI
    sRenderEngine->setView(mGui->getView());
    sRenderEngine->draw(sf::Sprite(mRenderTexture.getTexture()));
    sRenderEngine->draw(*mGui);
}

void GameStateEditor::exit()
{
    // Unsubscribe to inputs
    mGui->setListen(false);
}

void GameStateEditor::newGame(std::string cityName, uint64_t seed)
{
    // Create a new city
    mCity.createMap(std::move(cityName), seed);
    float width = (mCity.getMap().getWidth() + mCity.getMap().getHeight()) * Tile::HEIGHT;
    mGameView.setCenter(width * 0.5f, width * 0.25f);
    zoom(8.0f);

    // Subscribe to the city
    mCity.setGameMessageBus(sMessageBus);
    mCity.subscribe(mMailbox.getId());
}

void GameStateEditor::loadGame(const std::string& cityName)
{
    // Load city
    sSaveManager->load(cityName, mCity);
    float width = (mCity.getMap().getWidth() + mCity.getMap().getHeight()) * Tile::HEIGHT;
    mGameView.setCenter(width * 0.5f, width * 0.25f);
    zoom(8.0f);

    // Subscribe to the city
    mCity.setGameMessageBus(sMessageBus);
    mCity.subscribe(mMailbox.getId());
}

const sf::Texture& GameStateEditor::getCityTexture() const
{
    return mRenderTexture.getTexture();
}

void GameStateEditor::drawCity(sf::RenderTexture& renderTexture, const sf::View& view, bool background)
{
    renderTexture.clear(sf::Color::Transparent);
    if (background)
    {
        renderTexture.setView(sf::View(mBackground.getGlobalBounds()));
        renderTexture.draw(mBackground);
    }
    renderTexture.setView(view);
    renderTexture.draw(mCity);
    renderTexture.display();
}

void GameStateEditor::generatePreview(sf::Vector2u size, sf::Texture& texture)
{
    // Prepare to render
    sf::RenderTexture renderTexture;
    renderTexture.create(size.x, size.y);
    sf::View view = mGameView;
    float width = (mCity.getMap().getWidth() + mCity.getMap().getHeight()) * Tile::HEIGHT;
    view.setCenter(width * 0.5f, width * 0.25f);
    float factor = 1.05;
    view.setSize(factor * width, factor * width * float(size.y) / float(size.x));
    // Render
    drawCity(renderTexture, view, false);
    // Save
    texture = renderTexture.getTexture();
}

void GameStateEditor::createGui()
{
    // Tab buttons
    GuiWidget* buttonsWidget = mGui->get("tabButtons");
    for (GuiWidget* widget : buttonsWidget->getChildren())
        widget->subscribe(mMailbox.getId());
    // Tile buttons
    generateMenuTextures();
    GuiWidget* tabWidget = mGui->get<GuiTabWidget>("tabs");
    for (GuiWidget* widget : tabWidget->getChildren())
    {
        buttonsWidget = widget->getChildren()[1];
        for (GuiWidget* button : buttonsWidget->getChildren())
        {
            // Subscribe
            button->subscribe(mMailbox.getId());
            // Set image
            GuiImage* image = static_cast<GuiImage*>(button->getChildren()[0]);
            const std::string& name = button->getName();
            Tile::Type type = Tile::stringToType(name.substr(0, name.size() - 6));
            sf::Sprite sprite(mMenuTextures[static_cast<int>(type)]->getTexture());
            image->setSprite(sprite);
        }
    }

    updateTabs("landscapeTabButton");

    // Subscribe
    mGui->get("openImmigrantsWindowButton")->subscribe(mMailbox.getId());
    mGui->get("openCitizensWindowButton")->subscribe(mMailbox.getId());
    mGui->get("openRentalMarketWindowButton")->subscribe(mMailbox.getId());
    mGui->get("openLaborMarketWindowButton")->subscribe(mMailbox.getId());
    mGui->get("openGoodsMarketWindowButton")->subscribe(mMailbox.getId());
    mGui->get("openPoliciesWindowButton")->subscribe(mMailbox.getId());

    // Window managers
    mWindowManagers.emplace_back(mMailbox.getId());
    mWindowManagers.emplace_back(mMailbox.getId());
}

void GameStateEditor::startPanning(sf::Vector2i mousePosition)
{
    mActionState = ActionState::PANNING;
    mPanningAnchor = mousePosition;
}

void GameStateEditor::pan(sf::Vector2i mousePosition)
{
    sf::Vector2f delta(mousePosition - mPanningAnchor);
    mGameView.move(-1.0f * delta * mZoomLevel);
    mPanningAnchor = mousePosition;
}

void GameStateEditor::startSelecting(sf::Vector2f gamePosition)
{
    mActionState = ActionState::SELECTING;
    mSelectionStart = mCity.toTileIndices(gamePosition);
}

void GameStateEditor::stopSelecting()
{
    mCity.getMap().deselect();
    mGui->get("selectionCostText")->setVisible(false);
}

void GameStateEditor::generateMenuTextures()
{
    const Tile* neighbors[3][3];
    for (int i = 0; i < 3; ++i)
        std::fill(neighbors[i], neighbors[i] + 3, Map::getTileAtlas()[static_cast<int>(Tile::Type::EMPTY)].get());
    neighbors[2][1] = Map::getTileAtlas()[static_cast<int>(Tile::Type::ROAD_GRASS)].get();
    for (const std::unique_ptr<Tile>& tile : Map::getTileAtlas())
    {
        tile->setPosition(sf::Vector2i(), sf::Vector2f());
        tile->updateVariant(neighbors);
        std::unique_ptr<sf::RenderTexture> texture = std::make_unique<sf::RenderTexture>();
        texture->create(26, 26);
        sf::FloatRect bounds = tile->getBounds();
        sf::Vector2f center = sf::Vector2f(bounds.left, bounds.top) + 0.5f * sf::Vector2f(bounds.width, bounds.height);
        float size = std::max(bounds.width, bounds.height);
        sf::View view(center, sf::Vector2f(size, size));
        texture->setView(view);
        texture->clear(sf::Color::Transparent);
        texture->draw(*tile);
        texture->display();
        mMenuTextures.push_back(std::move(texture));
    }
}

void GameStateEditor::openPersonWindow(const Person& person)
{
    mWindowManagers[0].addWindow(mGui->createRootWithDefaultName<PersonWindow>(sStylesheetManager, person, mCity.getYear()));
}

void GameStateEditor::openBuildingWindow(const Building& building)
{
    mWindowManagers[1].addWindow(mGui->createRootWithDefaultName<BuildingWindow>(sStylesheetManager, building));
}

void GameStateEditor::openImmigrantsWindow()
{
    if (!mImmigrantsWindow)
        mImmigrantsWindow = mGui->createRootWithDefaultName<ImmigrantsWindow>(mMailbox.getId(), sMessageBus, sStylesheetManager, mCity);
}

void GameStateEditor::openCitizensWindow()
{
    if (!mCitizensWindow)
        mCitizensWindow = mGui->createRootWithDefaultName<CitizensWindow>(mMailbox.getId(), sMessageBus, sStylesheetManager, mCity);
}

void GameStateEditor::openRentalMarketWindow()
{
    if (!mRentalMarketWindow)
    {
        mRentalMarketWindow = mGui->createRootWithDefaultName<RentalMarketWindow>(sMessageBus, sStylesheetManager, static_cast<Market<Lease>*>(mCity.getMarket(MarketBase::Type::RENT)));
        mRentalMarketWindow->subscribe(mMailbox.getId());
    }
}

void GameStateEditor::openLaborMarketWindow()
{
    if (!mLaborMarketWindow)
    {
        mLaborMarketWindow = mGui->createRootWithDefaultName<LaborMarketWindow>(sMessageBus, sStylesheetManager, static_cast<Market<Work>*>(mCity.getMarket(MarketBase::Type::WORK)));
        mLaborMarketWindow->subscribe(mMailbox.getId());
    }
}

void GameStateEditor::openGoodsMarketWindow()
{
    if (!mGoodsMarketWindow)
    {
        std::array<Market<Good>*, 3> markets = {
            static_cast<Market<Good>*>(mCity.getMarket(MarketBase::Type::NECESSARY_GOOD)),
            static_cast<Market<Good>*>(mCity.getMarket(MarketBase::Type::NORMAL_GOOD)),
            static_cast<Market<Good>*>(mCity.getMarket(MarketBase::Type::LUXURY_GOOD))
        };
        mGoodsMarketWindow = mGui->createRootWithDefaultName<GoodsMarketWindow>(sMessageBus, sStylesheetManager, std::move(markets));
        mGoodsMarketWindow->subscribe(mMailbox.getId());
    }
}


void GameStateEditor::openPoliciesWindow()
{
    if (!mPoliciesWindow)
    {
        mPoliciesWindow = mGui->createRootWithDefaultName<PoliciesWindow>(sMessageBus, sStylesheetManager, mCity);
        mPoliciesWindow->subscribe(mMailbox.getId());
    }
}

void GameStateEditor::updateWindows()
{
    if (mImmigrantsWindow)
        mImmigrantsWindow->update();
    if (mCitizensWindow)
        mCitizensWindow->update();
    if (mRentalMarketWindow)
        mRentalMarketWindow->update();
    if (mLaborMarketWindow)
        mLaborMarketWindow->update();
    if (mGoodsMarketWindow)
        mGoodsMarketWindow->update();
    if (mPoliciesWindow)
        mPoliciesWindow->update();
    for (GuiWindow* window : mWindowManagers[0].getWindows())
    {
        PersonWindow* personWindow = static_cast<PersonWindow*>(window);
        drawCity(personWindow->getRenderTexture(), personWindow->getView(), true);
        personWindow->update();
    }
    for (GuiWindow* window : mWindowManagers[1].getWindows())
    {
        BuildingWindow* buildingWindow = static_cast<BuildingWindow*>(window);
        drawCity(buildingWindow->getRenderTexture(), buildingWindow->getView(), true);
        buildingWindow->update();
    }
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
    if ((mZoomLevel >= 2.0f && factor <= 1.0f) ||
        (mZoomLevel <= 8.0f && factor >= 1.0f))
    {
        mGameView.zoom(factor);
        mZoomLevel *= factor;
    }
}

Money GameStateEditor::getCost(Tile::Type type) const
{
    switch (type)
    {
        case Tile::Type::GRASS:
            return Money(50.0);
        case Tile::Type::FOREST:
            return Money(100.0);
        case Tile::Type::WATER:
            return Money(50.0);
        case Tile::Type::DIRT:
            return Money(50.0);
        case Tile::Type::CFB_HOUSING:
            return Money(0.0);
        case Tile::Type::AFFORDABLE_HOUSING:
            return Money(1000.0);
        case Tile::Type::APARTMENT_BUILDING:
            return Money(2000.0);
        case Tile::Type::VILLA:
            return Money(3000.0);
        case Tile::Type::CFB_INDUSTRY:
            return Money(0.0);
        case Tile::Type::FARM:
            return Money(1000.0);
        case Tile::Type::FACTORY:
            return Money(3000.0);
        case Tile::Type::WORKSHOP:
            return Money(2000.0);
        case Tile::Type::CFB_BUSINESS:
            return Money(0.0);
        case Tile::Type::GROCERY:
            return Money(1000.0);
        case Tile::Type::MALL:
            return Money(3000.0);
        case Tile::Type::BOUTIQUE:
            return Money(2000.0);
        case Tile::Type::CFB_HOSPITAL:
            return Money(0.0);
        case Tile::Type::HOSPITAL:
            return Money(2000.0);
        case Tile::Type::CFB_POLICE_STATION:
            return Money(0.0);
        case Tile::Type::POLICE_STATION:
            return Money(2000.0);
        case Tile::Type::CFB_SCHOOL:
            return Money(0.0);
        case Tile::Type::SCHOOL:
            return Money(2000.0);
        case Tile::Type::ROAD_GRASS:
            return Money(100.0);
        case Tile::Type::ROAD_SIDEWALK:
            return Money(200.0);
        case Tile::Type::BRIDGE:
            return Money(1000.0);
        default:
            return Money(0.0);
    }
}

Money GameStateEditor::computeCostOfSelection() const
{
    return Money(getCost(mCurrentTile) * mCity.getMap().getNbSelected());
}

Id GameStateEditor::extractId(const std::string& name, const std::string& prefix) const
{
    return std::atoi(name.substr(prefix.size(), name.find("|", prefix.size()) - prefix.size()).c_str());
}

void GameStateEditor::onNewYear()
{
    for (GuiWindow* window : mWindowManagers[0].getWindows())
        static_cast<PersonWindow*>(window)->onNewYear();
}
