#include "gui/GuiText.h"
#include "resource/XmlDocument.h"

GuiText::GuiText(const std::string& text, unsigned int characterSize, const XmlDocument* style) :
    mText(text, style->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"), characterSize)
{
    mText.setFillColor(style->getFirstChildByName("text").getAttributes().get<sf::Color>("color"));
    computeSize();
}

GuiText::GuiText(const PropertyList& properties) : GuiWidget(properties)
{
    const XmlDocument* style = properties.get<const XmlDocument*>("style");
    mText.setString(properties.get<std::string>("text", ""));
    mText.setFont(style->getFirstChildByName("text").getAttributes().get<const sf::Font&>("font"));
    mText.setCharacterSize(properties.get<unsigned int>("characterSize", 0));
    mText.setFillColor(style->getFirstChildByName("text").getAttributes().get<sf::Color>("color"));
    computeSize();
}

GuiText::~GuiText()
{
    //dtor
}

void GuiText::setPosition(sf::Vector2f position)
{
    sf::Vector2f offset(sf::Vector2f(mText.getGlobalBounds().left, mText.getGlobalBounds().top) - mText.getPosition());
    position = sf::Vector2f(sf::Vector2i(position));
    GuiWidget::setPosition(position);
    mText.setPosition(position - offset);
}

void GuiText::setCharacterSize(unsigned int characterSize)
{
    mText.setCharacterSize(characterSize);
    computeSize();
}

void GuiText::setText(const std::string& text)
{
    mText.setString(text);
    computeSize();
    setDirty();
}

void GuiText::setColor(sf::Color color)
{
    mText.setColor(color);
}

void GuiText::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mText);
}

void GuiText::computeSize()
{
    mSize = sf::Vector2f(mText.getGlobalBounds().width, mText.getGlobalBounds().height);
}
