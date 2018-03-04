#include "game/GameStateEditor.h"
#include <utility>
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "resource/GuiManager.h"
#include "gui/GuiButton.h"
#include "gui/GuiText.h"

GameStateEditor::GameStateEditor() :
    mCity("saves/city"), mActionState(ActionState::NONE), mZoomLevel(1.0f),
    mCurrentTile(Tile::Type::GRASS), mGui(sGuiManager->getGui("editor"))
{
    // Views
    sf::Vector2f windowSize = sf::Vector2f(sRenderEngine->getWindow().getSize());
    mGuiView.setSize(windowSize);
    mGameView.setSize(windowSize);
    mGuiView.setCenter(windowSize * 0.5f);
    mGameView.setCenter(sf::Vector2f(mCity.getMap().getWidth() * Tile::SIZE,
        mCity.getMap().getHeight() * Tile::SIZE * 0.5f));
    zoom(8.0f);

    // Background
    mBackground.setTexture(sTextureManager->getTexture("background"));

    // Gui
    createGui();

    // Subscribe to inputs
    sInputEngine->subscribe(mMailbox.getId());
}

GameStateEditor::~GameStateEditor()
{
    sInputEngine->unsubscribe(mMailbox.getId());
}

void GameStateEditor::draw(const float dt)
{
    sRenderEngine->clear();

    sRenderEngine->setView(mGuiView);
    sRenderEngine->draw(mBackground);

    sRenderEngine->setView(mGameView);
    sRenderEngine->draw(mCity.getMap());

    sRenderEngine->draw(*mGui);
}

void GameStateEditor::update(const float dt)
{
    mCity.update(dt);

    // Update the info bar at the bottom of the screen
    mGui->get<GuiText>("dayText")->setText("Day: " + std::to_string(mCity.getDay()));
    mGui->get<GuiText>("fundsText")->setText("$" + std::to_string(mCity.getFunds()));
    mGui->get<GuiText>("populationText")->setText("Population: " + std::to_string(mCity.getPopulation()));
    mGui->get<GuiText>("employmentText")->setText("Unemployment: " + std::to_string(mCity.getUnemployed()));
    mGui->get<GuiText>("currentTileText")->setText(Tile::typeToString(mCurrentTile));
}

void GameStateEditor::handleMessages()
{
    mGui->update();

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
                        sRenderEngine->getWindow().close();
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
                        mSelectionEnd.x = gamePos.y / Tile::SIZE + 0.5f * (gamePos.x / Tile::SIZE - mCity.getMap().getWidth() - 1);
                        mSelectionEnd.y = gamePos.y / Tile::SIZE - 0.5f * (gamePos.x / Tile::SIZE - mCity.getMap().getWidth() - 1);

                        mCity.getMap().deselect();
                        if(mCurrentTile == Tile::Type::GRASS)
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, {mCurrentTile, Tile::Type::WATER});
                        else
                        {
                            mCity.getMap().select(mSelectionStart, mSelectionEnd, {
                                mCurrentTile, Tile::Type::FOREST, Tile::Type::WATER, Tile::Type::ROAD,
                                Tile::Type::RESIDENTIAL, Tile::Type::COMMERCIAL, Tile::Type::INDUSTRIAL});
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
                        mGui->get("rightClickMenu")->setVisible(false);
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
                        else
                        {
                            // Open the tile select menu
                            sf::Vector2f menuPosition(mousePosition);
                            sf::Vector2f menuSize = mGui->get("rightClickMenu")->getSize();
                            sf::Vector2f windowSize(sRenderEngine->getWindow().getSize());
                            if(menuPosition.x > windowSize.x - menuSize.x)
                                menuPosition.x -= menuSize.x;
                            if(menuPosition.y > windowSize.y - menuSize.y)
                                menuPosition.y -= menuSize.y;
                            mGui->get("rightClickMenu")->setPosition(menuPosition);
                            mGui->get("rightClickMenu")->setVisible(true);
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        mGui->get("rightClickMenu")->setVisible(false);
                        // Select map tile
                        if (mActionState != ActionState::SELECTING)
                        {
                            mActionState = ActionState::SELECTING;
                            mSelectionStart.x = gamePos.y / Tile::SIZE + 0.5f * (gamePos.x / Tile::SIZE - mCity.getMap().getWidth() - 1);
                            mSelectionStart.y = gamePos.y / Tile::SIZE - 0.5f * (gamePos.x / Tile::SIZE - mCity.getMap().getWidth() - 1);
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
        if (message.type == MessageType::GUI && message.hasInfo())
        {
            // Select a context menu button
            mCurrentTile = Tile::stringToType(message.getInfo<std::string>());
        }
    }
}

void GameStateEditor::createGui()
{
    /*// Right bar
    auto roadMenu = mGui.create<GuiButton>("roadMenu", sStylesheetManager->getStylesheet("button"),
        "R", sf::Vector2f(32 , 32), 12, Message(MessageType::GUI));

    auto rightBar = mGui.createRoot<GuiWidget>("rightBar");
    rightBar->add(roadMenu);*/

    mGui->setWindowSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    mGui->get("infoBar")->setSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    mGui->get("rightClickMenu")->setVisible(false);
    mGui->get("rightClickMenu")->fitSizeToContent();

    mGui->get<GuiButton>("grassButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("forestButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("residentialButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("commercialButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("industrialButton")->subscribe(mMailbox.getId());
    mGui->get<GuiButton>("roadButton")->subscribe(mMailbox.getId());
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
        case Tile::Type::ROAD: return 100;
        default: return 0;
    }
}

unsigned int GameStateEditor::computeCostOfSelection() const
{
    return getCost(mCurrentTile) * mCity.getMap().getNbSelected();
}
