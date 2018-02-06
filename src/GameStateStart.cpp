#include "GameStateStart.h"
#include "GameStateEditor.h"

GameStateStart::GameStateStart(Game* game) : GameState(game)
{
    sf::Vector2f pos = sf::Vector2f(mGame->getWindow().getSize());
    mView.setSize(pos);
    pos *= 0.5f;
    mView.setCenter(pos);

    // Gui
    mGuiSystem.emplace("menu", Gui(sf::Vector2f(192, 32), 4, false, game->getStylesheet("button"),
        {std::make_pair("Load Game", "load_game")}));

    mGuiSystem.at("menu").setPosition(pos);
    mGuiSystem.at("menu").setOrigin(96, 32 * 0.5f);
    mGuiSystem.at("menu").show();
}

void GameStateStart::draw(const float dt)
{
    mGame->getWindow().setView(mView);

    mGame->getWindow().clear(sf::Color::Black);
    mGame->getWindow().draw(mGame->getBackground());

    for (auto gui : mGuiSystem)
        mGame->getWindow().draw(gui.second);
}

void GameStateStart::update(const float dt)
{

}

void GameStateStart::handleInput()
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
            case sf::Event::MouseMoved:
                // Highlight menu items
                mGuiSystem.at("menu").highlight(mGuiSystem.at("menu").getEntry(mGame->getWindow().mapPixelToCoords(mousePosition, mView)));
                break;
            case sf::Event::MouseButtonPressed:
                // Click on menu items
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    std::string message = mGuiSystem.at("menu").activate(mGame->getWindow().mapPixelToCoords(mousePosition, mView));
                    if(message == "load_game")
                        loadGame();
                }
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
