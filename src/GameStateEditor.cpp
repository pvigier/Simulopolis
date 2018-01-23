#include "GameStateEditor.h"

GameStateEditor::GameStateEditor(Game* game) : GameState(game)
{
    sf::Vector2f pos = sf::Vector2f(mGame->getWindow().getSize());
    mGuiView.setSize(pos);
    mGameView.setSize(pos);
    pos *= 0.5f;
    mGuiView.setCenter(pos);
    mGameView.setCenter(pos);
}

void GameStateEditor::draw(const float dt)
{
    mGame->getWindow().clear(sf::Color::Black);
    mGame->getWindow().draw(mGame->background);
}

void GameStateEditor::update(const float dt)
{

}

void GameStateEditor::handleInput()
{
    sf::Event event;
    while(mGame->getWindow().pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                mGame->getWindow().close();
                break;
            case sf::Event::Resized:
                mGameView.setSize(event.size.width, event.size.height);
                mGuiView.setSize(event.size.width, event.size.height);
                mGame->background.setPosition(mGame->getWindow().mapPixelToCoords(sf::Vector2i(0, 0), mGuiView));
                mGame->background.setScale(
                    float(event.size.width) / float(mGame->background.getTexture()->getSize().x),
                    float(event.size.height) / float(mGame->background.getTexture()->getSize().y));
                break;
            default:
                break;
        }
    }
}
