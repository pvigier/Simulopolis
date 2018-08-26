#include "gui/GuiText.h"
#include "resource/XmlDocument.h"

GuiText::GuiText(const sf::String& text, unsigned int characterSize, const XmlDocument* style) :
    GuiWidget(style),
    mText(text, mStyle->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"), characterSize)
{
    mText.setFillColor(mStyle->getFirstChildByName("text").getAttributes().get<sf::Color>("color"));
    setFixedSize(computeSize());
}

GuiText::GuiText(const PropertyList& properties) : GuiWidget(properties)
{
    const XmlDocument* style = properties.get<const XmlDocument*>("style");
    mText.setString(properties.get<sf::String>("text", ""));
    mText.setFont(style->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"));
    mText.setCharacterSize(properties.get<unsigned int>("characterSize", 0));
    mText.setFillColor(style->getFirstChildByName("text").getAttributes().get<sf::Color>("color"));
    setFixedSize(computeSize());
}

GuiText::~GuiText()
{
    //dtor
}

void GuiText::setCharacterSize(unsigned int characterSize)
{
    mText.setCharacterSize(characterSize);
    setFixedSize(computeSize());
}

const sf::Text& GuiText::getText() const
{
    return mText;
}

const sf::String& GuiText::getString() const
{
    return mText.getString();
}

void GuiText::setString(const sf::String& text)
{
    if (text != mText.getString())
    {
        mText.setString(text);
        setFixedSize(computeSize());
    }
}

void GuiText::setColor(sf::Color color)
{
    mText.setFillColor(color);
}

void GuiText::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::render(target, states);
    target.draw(mText, states);
}

void GuiText::onPositionChanged()
{
    GuiWidget::onPositionChanged();
    mText.setPosition(sf::Vector2f(sf::Vector2i(mPosition)));
}

sf::Vector2f GuiText::computeSize() const
{
    return sf::Vector2f(mText.getGlobalBounds().width, mText.getCharacterSize() * 5 / 4);
}
