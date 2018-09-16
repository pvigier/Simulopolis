/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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
        notify(Message::create(MessageType::INPUT, InputEvent(event)));
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
