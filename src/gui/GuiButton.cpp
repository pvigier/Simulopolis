#include "gui/GuiButton.h"
#include "resource/XmlDocument.h"

GuiButton::GuiButton(sf::Vector2f size, Message message, const XmlDocument* style) :
    mStyle(style), mMessage(message)
{
    setSize(size);
    mShape.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    setState(State::NORMAL);
}

GuiButton::GuiButton(const PropertyList& properties) :
    GuiWidget(properties), mMessage(Message::create(MessageType::GUI))
{
    mMessage = Message::create(MessageType::GUI, properties.get<std::string>("message", ""));
    mStyle = properties.get<const XmlDocument*>("style");
    mShape.setPosition(mPosition);
    mShape.setSize(mSize);
    mShape.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    setState(State::NORMAL);
}

void GuiButton::setPosition(sf::Vector2f position)
{
    GuiWidget::setPosition(position);
    mShape.setPosition(position);
}

void GuiButton::setSize(sf::Vector2f size)
{
    GuiWidget::setSize(size);
    mShape.setSize(size);
}

bool GuiButton::onHover(sf::Vector2f position)
{
    if (hitButton(position))
    {
        if (mState == State::NORMAL)
            setState(State::HOVERED);
    }
    else if (mState != State::FORCE_PRESSED && mState != State::DISABLED)
        setState(State::NORMAL);
    return false;
}

bool GuiButton::onPress(sf::Vector2f position)
{
    if (mState != State::DISABLED && hitButton(position))
        setState(State::PRESSED);
    return false;
}

bool GuiButton::onRelease(sf::Vector2f position)
{
    if (hitButton(position))
    {
        if (mState != State::DISABLED)
            notify(mMessage);
    }
    if (mState != State::FORCE_PRESSED && mState != State::DISABLED)
        setState(State::NORMAL);
    return false;
}

Message GuiButton::getMessage() const
{
    return mMessage;
}

void GuiButton::setState(State state)
{
    mState = state;
    sf::Color bodyColor = mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("color");
    sf::Color borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color");
    if (state == State::HOVERED)
    {
        bodyColor = mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("hoverColor", bodyColor);
        borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("hoverColor", borderColor);
    }
    else if (state == State::PRESSED || state == State::FORCE_PRESSED)
    {
        bodyColor = mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("pressColor", bodyColor);
        borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("pressColor", borderColor);
    }
    else if (state == State::DISABLED)
    {
        bodyColor = mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("disableColor", bodyColor);
        borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("disableColor", borderColor);
    }
    mShape.setFillColor(bodyColor);
    mShape.setOutlineColor(borderColor);
}

bool GuiButton::hitButton(sf::Vector2f position) const
{
    return mShape.getGlobalBounds().contains(position);
}

void GuiButton::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape);
}
