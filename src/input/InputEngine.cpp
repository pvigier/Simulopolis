#include "input/InputEngine.h"

InputEngine::InputEngine() : mRenderEngine(nullptr)
{
    //ctor
}

InputEngine::~InputEngine()
{
    //dtor
}

void InputEngine::setRenderEngine(RenderEngine* renderEngine)
{
    mRenderEngine = renderEngine;
}

void InputEngine::pollEvents()
{
    sf::Event event;
    while (mRenderEngine->getWindow().pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
                event.mouseButton.x -= mRenderEngine->getViewportOffset().x;
                event.mouseButton.y -= mRenderEngine->getViewportOffset().y;
                break;
            case sf::Event::MouseMoved:
                event.mouseMove.x -= mRenderEngine->getViewportOffset().x;
                event.mouseMove.y -= mRenderEngine->getViewportOffset().y;
                break;
            case sf::Event::MouseWheelMoved:
                event.mouseWheel.x -= mRenderEngine->getViewportOffset().x;
                event.mouseWheel.y -= mRenderEngine->getViewportOffset().y;
                break;
            default:
                break;
        }
        notify(Message::create(MessageType::INPUT, event));
    }
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
    return sf::Mouse::getPosition(mRenderEngine->getWindow()) - mRenderEngine->getViewportOffset();
}
