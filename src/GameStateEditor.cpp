#include "GameStateEditor.h"

GameStateEditor::GameStateEditor(Game* game) :
    GameState(game), mMap("saves/city_map.dat", 64, 64, mGame->getTileAtlas()),
    mActionState(ActionState::NONE), mZoomLevel(1.0f), mCurrentTile(&mGame->getTileAtlas()["grass"])
{
    // Init the views
    sf::Vector2f windowSize = sf::Vector2f(mGame->getWindow().getSize());
    mGuiView.setSize(windowSize);
    mGameView.setSize(windowSize);
    mGuiView.setCenter(windowSize * 0.5f);
    mGameView.setCenter(sf::Vector2f(mMap.getWidth() * mMap.getTileSize(),
        mMap.getHeight() * mMap.getTileSize() * 0.5f));
}

void GameStateEditor::draw(const float dt)
{
    mGame->getWindow().clear(sf::Color::Black);

    mGame->getWindow().setView(mGuiView);
    mGame->getWindow().draw(mGame->getBackground());

    mGame->getWindow().setView(mGameView);
    mMap.draw(mGame->getWindow(), dt);
}

void GameStateEditor::update(const float dt)
{

}

void GameStateEditor::handleInput()
{
    sf::Event event;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(mGame->getWindow());
    while (mGame->getWindow().pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                mGame->getWindow().close();
                break;
            case sf::Event::Resized:
                // Maybe we could simplify this?
                mGameView.setSize(event.size.width, event.size.height);
                mGameView.zoom(mZoomLevel);
                mGuiView.setSize(event.size.width, event.size.height);
                mGame->getBackground().setPosition(mGame->getWindow().mapPixelToCoords(sf::Vector2i(0, 0), mGuiView));
                mGame->getBackground().setScale(
                    float(event.size.width) / float(mGame->getBackground().getTexture()->getSize().x),
                    float(event.size.height) / float(mGame->getBackground().getTexture()->getSize().y));
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    mGame->getWindow().close();
                break;
            case sf::Event::MouseMoved:
                // Pan the camera
                if (mActionState == ActionState::PANNING)
                {
                    sf::Vector2f delta(mousePosition - mPanningAnchor);
                    mGameView.move(-1.0f * delta * mZoomLevel);
                    mPanningAnchor = sf::Mouse::getPosition(mGame->getWindow());
                }
                // Select tiles
                else if(mActionState == ActionState::SELECTING)
                {
                    sf::Vector2f pos = mGame->getWindow().mapPixelToCoords(mousePosition, mGameView);
                    mSelectionEnd.x = pos.y / mMap.getTileSize() + 0.5f * (pos.x / mMap.getTileSize() - mMap.getWidth() - 1);
                    mSelectionEnd.y = pos.y / mMap.getTileSize() - 0.5f * (pos.x / mMap.getTileSize() - mMap.getWidth() - 1);

                    mMap.clearSelected();
                    if(mCurrentTile->getType() == TileType::GRASS)
                        mMap.select(mSelectionStart, mSelectionEnd, {mCurrentTile->getType(), TileType::WATER});
                    else
                    {
                        mMap.select(mSelectionStart, mSelectionEnd, {
                            mCurrentTile->getType(), TileType::FOREST, TileType::WATER, TileType::ROAD,
                            TileType::RESIDENTIAL, TileType::COMMERCIAL, TileType::INDUSTRIAL});
                    }
                }
                break;
            case sf::Event::MouseButtonPressed:
                // Start panning
                if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    if (mActionState != ActionState::PANNING)
                    {
                        mActionState = ActionState::PANNING;
                        mPanningAnchor = sf::Mouse::getPosition(mGame->getWindow());
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Select map tile
                    if (mActionState != ActionState::SELECTING)
                    {
                        mActionState = ActionState::SELECTING;
                        sf::Vector2f pos = mGame->getWindow().mapPixelToCoords(mousePosition, mGameView);
                        mSelectionStart.x = pos.y / mMap.getTileSize() + 0.5f * (pos.x / mMap.getTileSize() - mMap.getWidth() - 1);
                        mSelectionStart.y = pos.y / mMap.getTileSize() - 0.5f * (pos.x / mMap.getTileSize() - mMap.getWidth() - 1);
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    // Stop selecting
                    if(mActionState == ActionState::SELECTING)
                    {
                        mActionState = ActionState::NONE;
                        mMap.clearSelected();
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
                        mActionState = ActionState::NONE;
                        mMap.clearSelected();
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
