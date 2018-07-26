#include "gui/GuiButton.h"
#include "resource/XmlDocument.h"
#include "gui/GuiEvent.h"

GuiButton::GuiButton(const XmlDocument* style) : GuiWidget(style)
{
    setBorderSize(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    setState(State::NORMAL);
}

GuiButton::GuiButton(const PropertyList& properties) : GuiWidget(properties)
{
    setBorderSize(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    setState(State::NORMAL);
}

bool GuiButton::onHover(sf::Vector2f position, bool processed)
{
    if (!processed && hitButton(position))
    {
        if (mState == State::NORMAL)
            setState(State::HOVERED);
        return true;
    }
    else if (mState != State::FORCE_PRESSED && mState != State::DISABLED)
        setState(State::NORMAL);
    return false;
}

bool GuiButton::onPress(sf::Vector2f position, bool processed)
{
    if (!processed && hitButton(position))
    {
        if (mState != State::FORCE_PRESSED && mState != State::DISABLED)
            setState(State::PRESSED);
        return true;
    }
    return false;
}

bool GuiButton::onRelease(sf::Vector2f position, bool processed)
{
    if (!processed && hitButton(position))
    {
        if (mState == State::PRESSED)
            notify(Message::create(MessageType::GUI, GuiEvent(this, GuiEvent::Type::BUTTON_RELEASED)));
        processed = true;
    }
    if (mState != State::FORCE_PRESSED && mState != State::DISABLED)
        setState(State::NORMAL);
    return processed;
}

void GuiButton::setState(State state)
{
    mState = state;
    sf::Color backgroundColor = mStyle->getFirstChildByName("background").getAttributes().get<sf::Color>("color");
    sf::Color borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color");
    if (state == State::HOVERED)
    {
        backgroundColor = mStyle->getFirstChildByName("background").getAttributes().get<sf::Color>("hoverColor", backgroundColor);
        borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("hoverColor", borderColor);
    }
    else if (state == State::PRESSED || state == State::FORCE_PRESSED)
    {
        backgroundColor = mStyle->getFirstChildByName("background").getAttributes().get<sf::Color>("pressColor", backgroundColor);
        borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("pressColor", borderColor);
    }
    else if (state == State::DISABLED)
    {
        backgroundColor = mStyle->getFirstChildByName("background").getAttributes().get<sf::Color>("disableColor", backgroundColor);
        borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("disableColor", borderColor);
    }
    mBackground.setFillColor(backgroundColor);
    mBackground.setOutlineColor(borderColor);
}

bool GuiButton::hitButton(sf::Vector2f position) const
{
    return mBackground.getGlobalBounds().contains(position);
}
