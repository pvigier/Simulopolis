#include "GameStateEditor.h"

GameStateEditor::GameStateEditor(Game* game) :
    GameState(game), mMap("saves/city_map.dat", 64, 64, mGame->getTileAtlas()),
    mActionState(ActionState::NONE), mZoomLevel(1.0f)
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
                /* Pan the camera */
                if (mActionState == ActionState::PANNING)
                {
                    sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(mGame->getWindow()) - mPanningAnchor);
                    mGameView.move(-1.0f * pos * mZoomLevel);
                    mPanningAnchor = sf::Mouse::getPosition(mGame->getWindow());
                }
                break;
            case sf::Event::MouseButtonPressed:
                /* Start panning */
                if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    if (mActionState != ActionState::PANNING)
                    {
                        mActionState = ActionState::PANNING;
                        mPanningAnchor = sf::Mouse::getPosition(mGame->getWindow());
                    }
                }
                break;
            case sf::Event::MouseButtonReleased:
                /* Stop panning */
                if (event.mouseButton.button == sf::Mouse::Middle)
                    mActionState = ActionState::NONE;
                break;
            case sf::Event::MouseWheelMoved:
                /* Zoom the view */
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
