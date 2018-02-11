#include "game/GameStateEditor.h"
#include <utility>
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"

GameStateEditor::GameStateEditor() :
    GameState(), mCity("saves/city"),
    mActionState(ActionState::NONE), mZoomLevel(1.0f), mCurrentTile(&Map::getTileAtlas()["grass"])
{
    // Initialize the city
    mCity.shuffleTiles();

    // Views
    sf::Vector2f windowSize = sf::Vector2f(sRenderEngine->getWindow().getSize());
    mGuiView.setSize(windowSize);
    mGameView.setSize(windowSize);
    mGuiView.setCenter(windowSize * 0.5f);
    mGameView.setCenter(sf::Vector2f(mCity.getMap().getWidth() * TILE_SIZE,
        mCity.getMap().getHeight() * TILE_SIZE * 0.5f));

    // Background
    mBackground.setTexture(sTextureManager->getTexture("background"));

    // Gui
    createGui();

    // Subscribe to inputs
    sInputEngine->subscribe(mMailbox.getId());
}

void GameStateEditor::draw(const float dt)
{
    sRenderEngine->clear();

    sRenderEngine->setView(mGuiView);
    sRenderEngine->draw(mBackground);

    sRenderEngine->setView(mGameView);
    mCity.getMap().draw(sRenderEngine->getWindow(), dt);

    sRenderEngine->setView(mGuiView);
    for (auto gui : mGuiSystem)
        sRenderEngine->draw(gui.second);
}

void GameStateEditor::update(const float dt)
{
    mCity.update(dt);

    // Update the info bar at the bottom of the screen
    mGuiSystem.at("infoBar").setEntryText(0, "Day: " + std::to_string(mCity.getDay()));
    mGuiSystem.at("infoBar").setEntryText(1, "$" + std::to_string(long(mCity.getFunds())));
    mGuiSystem.at("infoBar").setEntryText(2, std::to_string(long(mCity.getPopulation())) + " (" + std::to_string(long(mCity.getHomeless())) + ")");
    mGuiSystem.at("infoBar").setEntryText(3, std::to_string(long(mCity.getEmployable())) + " (" + std::to_string(long(mCity.getUnemployed())) + ")");
    mGuiSystem.at("infoBar").setEntryText(4, tileTypeToStr(mCurrentTile->getType()));

    // Highlight entries of the right click context menu
    mGuiSystem.at("rightClickMenu").highlight(mGuiSystem.at("rightClickMenu").getEntry(sRenderEngine->getWindow().mapPixelToCoords(sf::Mouse::getPosition(sRenderEngine->getWindow()), mGuiView)));
}

void GameStateEditor::handleMessages()
{
    sf::Vector2i mousePosition = sInputEngine->getMousePosition();
    sf::Vector2f guiPos = sRenderEngine->getWindow().mapPixelToCoords(mousePosition, mGuiView);
    sf::Vector2f gamePos = sRenderEngine->getWindow().mapPixelToCoords(mousePosition, mGameView);

    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type != MessageType::INPUT)
            continue;
        sf::Event event = message.getInfo<sf::Event>();
        switch (event.type)
        {
            case sf::Event::Closed:
                sRenderEngine->closeWindow();
                break;
            case sf::Event::Resized:
                // Maybe we could simplify this?
                mGameView.setSize(event.size.width, event.size.height);
                mGameView.zoom(mZoomLevel);
                mGuiView.setSize(event.size.width, event.size.height);
                mGuiSystem.at("infoBar").setDimensions(sf::Vector2f(event.size.width / mGuiSystem.at("infoBar").getNbEntries(), 16));
                mGuiSystem.at("infoBar").setPosition(sRenderEngine->getWindow().mapPixelToCoords(sf::Vector2i(0, event.size.height - 16), mGuiView));
                mGuiSystem.at("infoBar").show();
                mBackground.setPosition(sRenderEngine->getWindow().mapPixelToCoords(sf::Vector2i(0, 0), mGuiView));
                mBackground.setScale(
                    float(event.size.width) / float(mBackground.getTexture()->getSize().x),
                    float(event.size.height) / float(mBackground.getTexture()->getSize().y));
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    sRenderEngine->closeWindow();
                break;
            case sf::Event::MouseMoved:
                // Pan the camera
                if (mActionState == ActionState::PANNING)
                {
                    sf::Vector2f delta(mousePosition - mPanningAnchor);
                    mGameView.move(-1.0f * delta * mZoomLevel);
                    mPanningAnchor = sf::Mouse::getPosition(sRenderEngine->getWindow());
                }
                // Select tiles
                else if(mActionState == ActionState::SELECTING)
                {
                    mSelectionEnd.x = gamePos.y / TILE_SIZE + 0.5f * (gamePos.x / TILE_SIZE - mCity.getMap().getWidth() - 1);
                    mSelectionEnd.y = gamePos.y / TILE_SIZE - 0.5f * (gamePos.x / TILE_SIZE - mCity.getMap().getWidth() - 1);

                    mCity.getMap().clearSelected();
                    if(mCurrentTile->getType() == TileType::GRASS)
                        mCity.getMap().select(mSelectionStart, mSelectionEnd, {mCurrentTile->getType(), TileType::WATER});
                    else
                    {
                        mCity.getMap().select(mSelectionStart, mSelectionEnd, {
                            mCurrentTile->getType(), TileType::FOREST, TileType::WATER, TileType::ROAD,
                            TileType::RESIDENTIAL, TileType::COMMERCIAL, TileType::INDUSTRIAL});
                    }
                    double totalCost = mCity.getMap().getNumSelected() * mCurrentTile->getCost();
                    mGuiSystem.at("selectionCostText").setEntryText(0, "$" + std::to_string(totalCost));
                    if (mCity.getFunds() <= totalCost)
                        mGuiSystem.at("selectionCostText").highlight(0);
                    else
                        mGuiSystem.at("selectionCostText").highlight(-1);
                    mGuiSystem.at("selectionCostText").setPosition(guiPos + sf::Vector2f(16, -16));
                    mGuiSystem.at("selectionCostText").show();
                }
                // Highlight entries of the right click context menu
                mGuiSystem.at("rightClickMenu").highlight(mGuiSystem.at("rightClickMenu").getEntry(guiPos));
                break;
            case sf::Event::MouseButtonPressed:
                // Start panning
                if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    mGuiSystem.at("rightClickMenu").hide();
                    mGuiSystem.at("selectionCostText").hide();
                    if (mActionState != ActionState::PANNING)
                    {
                        mActionState = ActionState::PANNING;
                        mPanningAnchor = sf::Mouse::getPosition(sRenderEngine->getWindow());
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Select a context menu entry
                    if (mGuiSystem.at("rightClickMenu").isVisible() == true)
                    {
                        std::string message = mGuiSystem.at("rightClickMenu").activate(guiPos);
                        if(message != "null")
                            mCurrentTile = &Map::getTileAtlas().at(message);
                        mGuiSystem.at("rightClickMenu").hide();
                    }
                    // Select map tile
                    if (mActionState != ActionState::SELECTING)
                    {
                        mActionState = ActionState::SELECTING;
                        mSelectionStart.x = gamePos.y / TILE_SIZE + 0.5f * (gamePos.x / TILE_SIZE - mCity.getMap().getWidth() - 1);
                        mSelectionStart.y = gamePos.y / TILE_SIZE - 0.5f * (gamePos.x / TILE_SIZE - mCity.getMap().getWidth() - 1);
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    // Stop selecting
                    if(mActionState == ActionState::SELECTING)
                    {
                        mActionState = ActionState::NONE;
                        mCity.getMap().clearSelected();
                    }
                    else
                    {
                        // Open the tile select menu
                        sf::Vector2f pos = guiPos;
                        if(pos.x > sRenderEngine->getWindow().getSize().x - mGuiSystem.at("rightClickMenu").getSize().x)
                            pos -= sf::Vector2f(mGuiSystem.at("rightClickMenu").getSize().x, 0);
                        if(pos.y > sRenderEngine->getWindow().getSize().y - mGuiSystem.at("rightClickMenu").getSize().y)
                            pos -= sf::Vector2f(0, mGuiSystem.at("rightClickMenu").getSize().y);
                        mGuiSystem.at("rightClickMenu").setPosition(pos);
                        mGuiSystem.at("rightClickMenu").show();
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
                        // Replace tiles if enough funds and a tile is selected
                        if (mCurrentTile != nullptr)
                        {
                            unsigned int totalCost = mCurrentTile->getCost() * mCity.getMap().getNumSelected();
                            if(mCity.getFunds() >= totalCost)
                            {
                                mCity.bulldoze(*mCurrentTile);
                                mCity.getFunds() -= totalCost;
                                mCity.tileChanged();
                            }
                        }
                        mGuiSystem.at("selectionCostText").hide();
                        mActionState = ActionState::NONE;
                        mCity.getMap().clearSelected();
                    }
                }
                break;
            case sf::Event::MouseWheelMoved:
                // Zoom the view
                if (event.mouseWheel.delta < 0)
                {
                    mGameView.zoom(2.0f);
                    mZoomLevel *= 2.0f;
                }
                else
                {
                    mGameView.zoom(0.5f);
                    mZoomLevel *= 0.5f;
                }
                break;
            default:
                break;
        }
    }
}

void GameStateEditor::createGui()
{
    // Create gui elements
    mGuiSystem.emplace("rightClickMenu", Gui(sf::Vector2f(196, 16), 2, false, sStylesheetManager->getStylesheet("button"),
        {
            std::make_pair("Flatten $" + std::to_string(Map::getTileAtlas()["grass"].getCost()), "grass"),
            std::make_pair("Forest $" + std::to_string(Map::getTileAtlas()["forest"].getCost()), "forest" ),
            std::make_pair("Residential Zone $" + std::to_string(Map::getTileAtlas()["residential"].getCost()), "residential"),
            std::make_pair("Commercial Zone $" + std::to_string(Map::getTileAtlas()["commercial"].getCost()), "commercial"),
            std::make_pair("Industrial Zone $" + std::to_string(Map::getTileAtlas()["industrial"].getCost()), "industrial"),
            std::make_pair("Road $" + std::to_string(Map::getTileAtlas()["road"].getCost()), "road")
        }));

    mGuiSystem.emplace("selectionCostText", Gui(sf::Vector2f(196, 16), 0, false, sStylesheetManager->getStylesheet("text"),
        { std::make_pair("", "") }));

    mGuiSystem.emplace("infoBar", Gui(sf::Vector2f(sRenderEngine->getWindow().getSize().x / 5 , 16), 2, true, sStylesheetManager->getStylesheet("button"),
        {
            std::make_pair("time",          "time"),
            std::make_pair("funds",         "funds"),
            std::make_pair("population",    "population"),
            std::make_pair("employment",    "employment"),
            std::make_pair("current tile",  "tile")
        }));
    mGuiSystem.at("infoBar").setPosition(sf::Vector2f(0, sRenderEngine->getWindow().getSize().y - 16));
    mGuiSystem.at("infoBar").show();
}
