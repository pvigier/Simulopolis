#include "game/GameStateEditor.h"
#include <utility>
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "resource/TextureManager.h"
#include "resource/StylesheetManager.h"
#include "gui/GuiButton.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"

GameStateEditor::GameStateEditor() :
    mCity("saves/city"), mActionState(ActionState::NONE), mZoomLevel(1.0f),
    mCurrentTile(Tile::Type::GRASS),
    mGui(sf::Vector2f(sRenderEngine->getWindow().getSize()))
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

    sRenderEngine->draw(mGui);
}

void GameStateEditor::update(const float dt)
{
    mCity.update(dt);

    // Update the info bar at the bottom of the screen
    mGui.get<GuiButton>("dayLabel")->setText("Day: " + std::to_string(mCity.getDay()));
    mGui.get<GuiButton>("fundsLabel")->setText("$" + std::to_string(long(mCity.getFunds())));
    mGui.get<GuiButton>("populationLabel")->setText("Population: " + std::to_string(mCity.getPopulation()));
    mGui.get<GuiButton>("employmentLabel")->setText("Unemployment: " + std::to_string(mCity.getUnemployed()));
    mGui.get<GuiButton>("currentTileLabel")->setText(tileTypeToStr(mCurrentTile));
}

void GameStateEditor::handleMessages()
{
    mGui.update();

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
                        std::shared_ptr<GuiButton> selectionCostText = mGui.get<GuiButton>("selectionCostText");
                        selectionCostText->setText("$" + std::to_string(totalCost));
                        selectionCostText->setHighlight(mCity.getFunds() < totalCost);
                        selectionCostText->setPosition(sf::Vector2f(mousePosition) + sf::Vector2f(16, -16));
                        selectionCostText->setVisible(true);
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    // Start panning
                    if (event.mouseButton.button == sf::Mouse::Middle)
                    {
                        mGui.get("rightClickMenu")->setVisible(false);
                        mGui.get("selectionCostText")->setVisible(false);
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
                            sf::Vector2f menuSize = mGui.get("rightClickMenu")->getSize();
                            sf::Vector2f windowSize(sRenderEngine->getWindow().getSize());
                            if(menuPosition.x > windowSize.x - menuSize.x)
                                menuPosition.x -= menuSize.x;
                            if(menuPosition.y > windowSize.y - menuSize.y)
                                menuPosition.y -= menuSize.y;
                            mGui.get("rightClickMenu")->setPosition(menuPosition);
                            mGui.get("rightClickMenu")->setVisible(true);
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        mGui.get("rightClickMenu")->setVisible(false);
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
                            mGui.get("selectionCostText")->setVisible(false);
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
            mCurrentTile = message.getInfo<Tile::Type>();
        }
    }
    /*while (!mMailbox.isEmpty())
    {
        switch (event.type)
        {
            case sf::Event::Resized:
                // Maybe we could simplify this?
                mGameView.setSize(event.size.width, event.size.height);
                mGameView.zoom(mZoomLevel);
                //mGuiView.setSize(event.size.width, event.size.height);
                //mGuiSystem.at("infoBar").setDimensions(sf::Vector2f(event.size.width / mGuiSystem.at("infoBar").getNbEntries(), 16));
                //mGuiSystem.at("infoBar").setPosition(sRenderEngine->getWindow().mapPixelToCoords(sf::Vector2i(0, event.size.height - 16), mGuiView));
                //mGuiSystem.at("infoBar").show();
                mBackground.setPosition(sRenderEngine->getWindow().mapPixelToCoords(sf::Vector2i(0, 0), mGuiView));
                mBackground.setScale(
                    float(event.size.width) / float(mBackground.getTexture()->getSize().x),
                    float(event.size.height) / float(mBackground.getTexture()->getSize().y));
                break;
        }
    }*/
}

void GameStateEditor::createGui()
{
    // Right click menu
    auto grassButton = mGui.create<GuiButton>("grassButton",
        sStylesheetManager->getStylesheet("button"), "Flatten $" + std::to_string(getCost(Tile::Type::GRASS)),
        sf::Vector2f(196, 16), 12, Message::create(MessageType::GUI, Tile::Type::GRASS));
    grassButton->subscribe(mMailbox.getId());

    auto forestButton = mGui.create<GuiButton>("forestButton",
        sStylesheetManager->getStylesheet("button"), "Forest $" + std::to_string(getCost(Tile::Type::FOREST)),
        sf::Vector2f(196, 16), 12, Message::create(MessageType::GUI, Tile::Type::FOREST));
    forestButton->subscribe(mMailbox.getId());

    auto residentialButton = mGui.create<GuiButton>("residentialButton",
        sStylesheetManager->getStylesheet("button"), "Residential Zone $" + std::to_string(getCost(Tile::Type::RESIDENTIAL)),
        sf::Vector2f(196, 16), 12, Message::create(MessageType::GUI, Tile::Type::RESIDENTIAL));
    residentialButton->subscribe(mMailbox.getId());

    auto commercialButton = mGui.create<GuiButton>("commercialButton", sStylesheetManager->getStylesheet("button"),
        "Commercial Zone $" + std::to_string(getCost(Tile::Type::COMMERCIAL)),
        sf::Vector2f(196, 16), 12, Message::create(MessageType::GUI, Tile::Type::COMMERCIAL));
    commercialButton->subscribe(mMailbox.getId());

    auto industrialButton = mGui.create<GuiButton>("industrialButton", sStylesheetManager->getStylesheet("button"),
        "Industrial Zone $" + std::to_string(getCost(Tile::Type::INDUSTRIAL)),
        sf::Vector2f(196, 16), 12, Message::create(MessageType::GUI, Tile::Type::INDUSTRIAL));
    industrialButton->subscribe(mMailbox.getId());

    auto roadButton = mGui.create<GuiButton>("roadButton", sStylesheetManager->getStylesheet("button"),
        "Road $" + std::to_string(getCost(Tile::Type::ROAD)),
        sf::Vector2f(196, 16), 12, Message::create(MessageType::GUI, Tile::Type::ROAD));
    roadButton->subscribe(mMailbox.getId());

    auto rightClickMenu = mGui.createRoot<GuiWidget>("rightClickMenu");
    rightClickMenu->add(grassButton);
    rightClickMenu->add(forestButton);
    rightClickMenu->add(residentialButton);
    rightClickMenu->add(commercialButton);
    rightClickMenu->add(industrialButton);
    rightClickMenu->add(roadButton);
    rightClickMenu->setLayout(GuiLayoutPtr(new GuiVBoxLayout()));
    rightClickMenu->fitSizeToContent();
    rightClickMenu->setVisible(false);

    auto selectionCostText = mGui.createRoot<GuiButton>("selectionCostText",
        sStylesheetManager->getStylesheet("text"), "", sf::Vector2f(196, 16), 12, Message(MessageType::GUI));

    // Info bar
    auto dayLabel = mGui.create<GuiButton>("dayLabel", sStylesheetManager->getStylesheet("button"),
        "", sf::Vector2f(sRenderEngine->getWindow().getSize().x / 5 , 16), 12, Message(MessageType::GUI));

    auto fundsLabel = mGui.create<GuiButton>("fundsLabel", sStylesheetManager->getStylesheet("button"),
        "", sf::Vector2f(sRenderEngine->getWindow().getSize().x / 5 , 16), 12, Message(MessageType::GUI));

    auto populationLabel = mGui.create<GuiButton>("populationLabel", sStylesheetManager->getStylesheet("button"),
        "", sf::Vector2f(sRenderEngine->getWindow().getSize().x / 5 , 16), 12, Message(MessageType::GUI));

    auto employmentLabel = mGui.create<GuiButton>("employmentLabel", sStylesheetManager->getStylesheet("button"),
        "", sf::Vector2f(sRenderEngine->getWindow().getSize().x / 5 , 16), 12, Message(MessageType::GUI));

    auto currentTileLabel = mGui.create<GuiButton>("currentTileLabel", sStylesheetManager->getStylesheet("button"),
        "", sf::Vector2f(sRenderEngine->getWindow().getSize().x / 5 , 16), 12, Message(MessageType::GUI));

    auto infoBar = mGui.createRoot<GuiWidget>("infoBar");
    infoBar->add(dayLabel);
    infoBar->add(fundsLabel);
    infoBar->add(populationLabel);
    infoBar->add(employmentLabel);
    infoBar->add(currentTileLabel);
    infoBar->setSize(sf::Vector2f(sRenderEngine->getWindow().getSize()));
    GuiHBoxLayout* infoBarLayout = new GuiHBoxLayout();
    infoBarLayout->setVAlignment(GuiLayout::VAlignment::Bottom);
    infoBar->setLayout(GuiLayoutPtr(new GuiHBoxLayout(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Bottom)));
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
