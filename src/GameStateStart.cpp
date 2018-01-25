#include "GameStateStart.h"
#include "GameStateEditor.h"

GameStateStart::GameStateStart(Game* game) : GameState(game)
{
    sf::Vector2f pos = sf::Vector2f(mGame->getWindow().getSize());
    mView.setSize(pos);
    pos *= 0.5f;
    mView.setCenter(pos);
}

void GameStateStart::draw(const float dt)
{
    mGame->getWindow().setView(mView);

    mGame->getWindow().clear(sf::Color::Black);
    mGame->getWindow().draw(mGame->getBackground());
}

void GameStateStart::update(const float dt)
{

}

void GameStateStart::handleInput()
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
                mView.setSize(event.size.width, event.size.height);
                mGame->getBackground().setPosition(mGame->getWindow().mapPixelToCoords(sf::Vector2i(0, 0)));
                mGame->getBackground().setScale(
                    float(event.size.width) / float(mGame->getBackground().getTexture()->getSize().x),
                    float(event.size.height) / float(mGame->getBackground().getTexture()->getSize().y));
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    mGame->getWindow().close();
                else if (event.key.code == sf::Keyboard::Space)
                    loadGame();
                break;
            default:
                break;
        }
    }
}

void GameStateStart::loadGame()
{
    mGame->pushState(new GameStateEditor(mGame));
}
