#include "input/InputEngine.h"

InputEngine::InputEngine() : mWindow(nullptr)
{
    //ctor
}

InputEngine::~InputEngine()
{
    //dtor
}

void InputEngine::setWindow(sf::Window* window)
{
    mWindow = window;
}

void InputEngine::pollEvents()
{
    sf::Event event;
    while (mWindow->pollEvent(event))
        notify(Message(UNDEFINED, UNDEFINED, MessageType::INPUT, std::make_shared<sf::Event>(event)));
}

bool InputEngine::isKeyPressed(sf::Keyboard::Key key) const
{
     return sf::Keyboard::isKeyPressed(key);
}

bool InputEngine::isButtonPressed(sf::Mouse::Button button) const
{
    return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i InputEngine::getMousePosition() const
{
    return sf::Mouse::getPosition(*mWindow);
}
