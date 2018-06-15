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
#include "gui/GuiWindow.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiEvent.h"

GameStateEditor::GameStateEditor() : mActionState(ActionState::NONE), mZoomLevel(1.0f),
    mCurrentTile(Tile::Type::GRASS), mGui(sGuiManager->getGui("editor"))
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
    mGui->get<GuiButton>("grassButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("forestButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("residentialButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("commercialButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("industrialButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("hospitalButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("policeButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("schoolButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadMenuButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadGrassButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadSidewalkButton")->unsubscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadWaterButton")->unsubscribe(mMailbox.getId());
    mGui->unsubscribe(mMailbox.getId());
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
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, {mCurrentTile, Tile::Type::WATER});
                        else if (mCurrentTile == Tile::Type::ROAD_WATER)
                        {
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, {
                                mCurrentTile, Tile::Type::GRASS, Tile::Type::FOREST, Tile::Type::DIRT,
                                Tile::Type::ROAD_GRASS, Tile::Type::ROAD_SIDEWALK, Tile::Type::ROAD_WATER,
                                Tile::Type::RESIDENTIAL, Tile::Type::COMMERCIAL, Tile::Type::INDUSTRIAL,
                                Tile::Type::HOSPITAL, Tile::Type::POLICE, Tile::Type::SCHOOL});
                        }
                        else
                        {
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, {
                                mCurrentTile, Tile::Type::FOREST, Tile::Type::WATER,
                                Tile::Type::ROAD_GRASS, Tile::Type::ROAD_SIDEWALK, Tile::Type::ROAD_WATER,
                                Tile::Type::RESIDENTIAL, Tile::Type::COMMERCIAL, Tile::Type::INDUSTRIAL,
                                Tile::Type::HOSPITAL, Tile::Type::POLICE, Tile::Type::SCHOOL});
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
                    closeMenus();
                    const std::string& name = event.widget->getName();
                    if (name == "roadMenuButton")
                    {
                        mGui->get("roadMenuButtons")->setVisible(true);
                        mGui->get<GuiButton>("roadMenuButton")->setState(GuiButton::State::FORCE_PRESSED);
                    }
                    else
                        mCurrentTile = Tile::stringToType(name.substr(0, name.size() - 6));
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
    mGui->get("infoBar")->setSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));

    mGui->get<GuiButton>("grassButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("forestButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("residentialButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("commercialButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("industrialButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("hospitalButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("policeButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("schoolButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadMenuButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadGrassButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadSidewalkButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadWaterButton")->subscribe(mMailbox.getId());
}

void GameStateEditor::createPersonWindow(const Person& person)
{
    std::string fullname = person.getFullName();
    auto window = mGui->createRoot<GuiWindow>(fullname + "Window", sf::Vector2f(200.0f, 120.0f), fullname, sStylesheetManager->getStylesheet("window"));
    auto firstNameText = mGui->create<GuiText>(fullname + "FirstNameText", "First name: " + person.getFirstName(), 10, sStylesheetManager->getStylesheet("text"));
    auto lastNameText = mGui->create<GuiText>(fullname + "LastNameText", "Last name: " + person.getLastName(), 10, sStylesheetManager->getStylesheet("text"));
    auto ageText = mGui->create<GuiText>(fullname + "AgeText", "Age: " + std::to_string(person.getAge(mCity.getYear())), 10, sStylesheetManager->getStylesheet("text"));
    auto stateText = mGui->create<GuiText>(fullname + "StateText", "State: " + std::to_string(static_cast<int>(person.getState())), 10, sStylesheetManager->getStylesheet("text"));
    auto sleepText = mGui->create<GuiText>(fullname + "SleepText", "Sleep: " + std::to_string(person.getSleep()), 10, sStylesheetManager->getStylesheet("text"));
    auto hygieneText = mGui->create<GuiText>(fullname + "HygieneText", "Hygiene: " + std::to_string(person.getHygiene()), 10, sStylesheetManager->getStylesheet("text"));
    auto safetyText = mGui->create<GuiText>(fullname + "SafetyText", "Safety: " + std::to_string(person.getSafety()), 10, sStylesheetManager->getStylesheet("text"));
    auto hungerText = mGui->create<GuiText>(fullname + "HungerText", "Hunger: " + std::to_string(person.getHunger()), 10, sStylesheetManager->getStylesheet("text"));
    auto happinessText = mGui->create<GuiText>(fullname + "HapinessText", "Happiness: " + std::to_string(person.getHappiness()), 10, sStylesheetManager->getStylesheet("text"));
    window->add(firstNameText);
    window->add(lastNameText);
    window->add(ageText);
    window->add(stateText);
    window->add(sleepText);
    window->add(hygieneText);
    window->add(safetyText);
    window->add(hungerText);
    window->add(happinessText);
    window->setPosition(sf::Vector2f(50.0f, 50.0f));
    window->setLayout(std::make_unique<GuiVBoxLayout>(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Top, 3.0f));
}

void GameStateEditor::createCompanyWindow(const Company& company)
{
    std::string name = company.getName();
    auto window = mGui->createRoot<GuiWindow>(name + "Window", sf::Vector2f(200.0f, 120.0f), name, sStylesheetManager->getStylesheet("window"));
    auto ownerText = mGui->create<GuiText>(name + "OwnerText", "Owner: " + company.getOwner()->getFullName(), 10, sStylesheetManager->getStylesheet("text"));
    auto yearText = mGui->create<GuiText>(name + "YearText", "Year: " + std::to_string(0), 10, sStylesheetManager->getStylesheet("text"));
    window->add(ownerText);
    window->add(yearText);
    window->setPosition(sf::Vector2f(200.0f, 50.0f));
    window->setLayout(std::make_unique<GuiVBoxLayout>(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Top, 3.0f));
}

void GameStateEditor::closeMenus()
{
    mGui->get("roadMenuButtons")->setVisible(false);
    mGui->get<GuiButton>("roadMenuButton")->setState(GuiButton::State::NORMAL);
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
        case Tile::Type::RESIDENTIAL: return 300;
        case Tile::Type::COMMERCIAL: return 300;
        case Tile::Type::INDUSTRIAL: return 300;
        case Tile::Type::ROAD_GRASS: return 100;
        default: return 0;
    }
}

unsigned int GameStateEditor::computeCostOfSelection() const
{
    return getCost(mCurrentTile) * mCity.getMap().getNbSelected();
}
